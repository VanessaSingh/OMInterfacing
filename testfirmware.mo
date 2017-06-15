model testfirmware
import sc=SerialCommunication;
Integer h(fixed=false), w(fixed=false), r(fixed=false), c(fixed=false);
algorithm
  h := sc.open_serial(1,2,9600);

    w := sc.write_serial(1, "v", 1);
    r := sc.read_serial(1,2);
  c := sc.close_serial(1);
end testfirmware;
