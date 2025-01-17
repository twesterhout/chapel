/*
 * Copyright 2023-2024 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "chpl/framework/Context.h"
#include "chpl/parsing/parsing-queries.h"
#include "iterator-support.h"
#include "error-tracker.h"
#include "core-types.h"
#include <utility>

static PyMethodDef ChapelMethods[] = {
  { NULL, NULL, 0, NULL } /* Sentinel */
};

static PyModuleDef ChapelModule {
  PyModuleDef_HEAD_INIT,
  .m_name="core",
  .m_doc="A Python bridge for the Chapel frontend library",
  .m_size=-1 /* Per-interpreter memory (not used currently) */,
  .m_methods=ChapelMethods,
};

extern "C" {

PyMODINIT_FUNC PyInit_core() {
  PyObject* chapelModule = nullptr;

  setupContextType();
  setupErrorType();
  setupErrorManagerType();
  setupLocationType();
  setupScopeType();
  setupAstIterType();
  setupAstCallIterType();
  setupAstNodeType();
  setupChapelTypeType();
  setupParamType();
  setupGeneratedTypes();

#define READY_TYPE(NAME) if (PyType_Ready(&NAME##Type) < 0) return nullptr;
#define GENERATED_TYPE(ROOT, NAME, TAG, FLAGS) READY_TYPE(NAME)
#include "generated-types-list.h"
#undef GENERATED_TYPE
  READY_TYPE(Context)
  READY_TYPE(Error)
  READY_TYPE(ErrorManager)
  READY_TYPE(Location)
  READY_TYPE(Scope)
  READY_TYPE(AstIter)
  READY_TYPE(AstCallIter)
  READY_TYPE(AstNode)
  READY_TYPE(ChapelType)
  READY_TYPE(Param)

  chapelModule = PyModule_Create(&ChapelModule);
  if (!chapelModule) return nullptr;

#define ADD_TYPE(NAME) if (PyModule_AddObject(chapelModule, #NAME, (PyObject*) &NAME##Type) < 0) return nullptr;
#define GENERATED_TYPE(ROOT, NAME, TAG, FLAGS) ADD_TYPE(NAME)
#include "generated-types-list.h"
#undef GENERATED_TYPE
  ADD_TYPE(AstNode);
  ADD_TYPE(ChapelType);
  ADD_TYPE(Param);

  if (PyModule_AddObject(chapelModule, "Context", (PyObject *) &ContextType) < 0) {
    Py_DECREF(&ContextType);
    Py_DECREF(chapelModule);
    return NULL;
  }
  return chapelModule;
}

}
