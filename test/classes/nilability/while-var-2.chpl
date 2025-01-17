// Test various successful scenarios of iterating over a linked list (owned)

// a concrete class
class clist {
  var elm: int;
  var next: owned clist?;
}

var globalCL = new clist?(30, new clist(31, new clist(32)));

// a generic class
class glist {
  var elm;
  var next: owned glist(elm.type)?;
}

proc showU1(name: string, list) {
  write("showU1 ", name, ":");
  pragma "unsafe" // one workaround for lifetime of 'cur'
  var curr = list: borrowed class?;
  while const cur = curr {
    write(" ", cur.elm);
    curr = cur.next;
  }
  writeln();
}

proc consume(name: string, in list) {
  write("consume ", name, ":");
  var curr = list: class?;
  while const cur = curr {
    write(" ", cur.elm);
    curr = cur.next;
  }
  writeln();
}

proc main {
  var cl = new clist(100, new clist(101, new clist(102)));
  var c0: owned clist? = nil;

  var gl = new glist("hi", new glist("there", new glist("world")));
  var g0 = nil: owned glist(locale)?;

  showU1("cl", cl);
  showU1("c0", c0);
  showU1("gl", gl);
  showU1("g0", g0);

  writeln("cl = ", cl);
  writeln("c0 = ", c0);
  writeln("gl = ", gl);
  writeln("g0 = ", g0);

  consume("cl", cl);
  consume("c0", c0);
  consume("gl", gl);
  consume("g0", g0);

  consume("globalCL", globalCL);
  assert(globalCL == nil);
}
