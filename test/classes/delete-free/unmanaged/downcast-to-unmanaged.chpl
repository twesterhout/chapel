class C {
  var x: int;
}

class D:C {
  var y: int;
}

var c:C = new D(1,1);

assert((c:borrowed C).type == borrowed C);
writeln(c:borrowed C);

assert((c:borrowed D).type == borrowed D);
writeln(c:borrowed D);
