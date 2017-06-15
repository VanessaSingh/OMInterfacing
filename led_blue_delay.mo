model led_blue_delay
  extends Modelica.Icons.Example;
  import sComm = SerialCommunication;
  import Exn = Modelica.Exceptions;
  import strm = Modelica.Utilities.Streams;
  Integer ok(fixed = false);
  Integer digital_out(fixed = false);
  Integer c_ok(fixed = false);
algorithm
  ok := sComm.open_serial(1, 2, 115200);// At port 2 with baudrate of 115200
  if ok <> 0 then
    strm.print("Check the serial port and try again");
  else
    digital_out := sComm.cmd_digital_out(1, 9, 1);// This will turn on the blue LED
    sComm.delay(2000);// Let the blue LED be on for two seconds
    digital_out := sComm.cmd_digital_out(1, 9, 0);// This will turn off the blue LED
  end if;
  c_ok := sComm.close_serial(1);
end led_blue_delay;
