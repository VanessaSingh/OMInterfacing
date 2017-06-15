model push
extends Modelica.Icons.Example;
  import sComm = SerialCommunication;
  import Exn = Modelica.Exceptions;
  import strm = Modelica.Utilities.Streams;
  Integer ok(fixed = false);
  Integer digital_in(fixed = false);
  Integer digital_out(fixed = false);
  Integer c_ok(fixed = false);
algorithm
  ok := sComm.open_serial(1, 2, 115200);
// At port 0 with baudrate of 115200
  if ok <> 0 then
    strm.print("Unable to open serial port, please check");
  else
    for i in 1:1000 loop
      digital_in := sComm.cmd_digital_in(1, 12);
      if digital_in == 0 then
        digital_out := sComm.cmd_digital_out(1, 11, 0);
      else
        digital_out := sComm.cmd_digital_out(1, 11, 1);
      end if;
    end for;
  end if;
// This will turn the blue LED
// This will turn the blue LED
  c_ok := sComm.close_serial(1);
// To close the connection safely
end push;
