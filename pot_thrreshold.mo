model pot_thrreshold
extends Modelica.Icons.Example;
    import sComm = SerialCommunication;
    import Exn = Modelica.Exceptions;
    import strm = Modelica.Utilities.Streams;
    Integer ok(fixed = false);
    Integer analog_in(fixed = false);
    Integer digital_out(fixed = false);
    Integer c_ok(fixed = false);
  algorithm
    ok := sComm.open_serial(1, 2, 115200);// At port 0 with baudrate of 115200
    if ok <> 0 then
      strm.print("Unable to open serial port, please check");
    else
      for i in 1:10 loop//Run for 10 iterations
        analog_in := sComm.cmd_analog_in(1, 2);// read analog pin 2
        if analog_in >= 0 and analog_in < 320 then// Threshold 1
          digital_out := sComm.cmd_digital_out(1, 11, 1);// Turn ON LED
          sComm.delay(1000);
          digital_out := sComm.cmd_digital_out(1, 11, 0);
        elseif analog_in >= 320 and analog_in <= 900 then//Threshold 2
          digital_out := sComm.cmd_digital_out(1, 10, 1);// Turn ON LED
          sComm.delay(1000);
          digital_out := sComm.cmd_digital_out(1, 10, 0);// Turn OFF LED
        elseif analog_in > 900 and analog_in <= 1023 then
          digital_out := sComm.cmd_digital_out(1, 9, 1);// Turn ON LED
          sComm.delay(1000);
          digital_out := sComm.cmd_digital_out(1, 9, 0);// Turn OFF LED
        end if;
      end for;
    end if;

    c_ok := sComm.close_serial(1);// To close the connection safely
end pot_thrreshold;
