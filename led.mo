model led
  extends Modelica.Icons.Example;
  import sComm = SerialCommunication;
  import Exn = Modelica.Exceptions;
  import strm = Modelica.Utilities.Streams;
  Integer ok(fixed = false);
  Integer digital_out(fixed = false);
  Integer c_ok(fixed = false);
algorithm
  ok := sComm.open_serial(1, 2, 115200);// At port 0 with baudrate of 115200
  if ok <> 0 then
    strm.print("Check the serial port and try again");
  else
    digital_out := sComm.cmd_digital_out(1, 9, 1);
  end if;// This will turn the blue LED
  c_ok := sComm.close_serial(1);// To close the connection safely
end led;
