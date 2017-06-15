model PE
  function try
    input Integer temp;
    output Integer OK;
  	external OK=helloworld(temp) 
  	annotation(Library="D:/interfacing/HW.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/hworld.h\"");
  end try;
  Integer OKAY(fixed=false);
  equation
  OKAY=try(3);
end PE;
