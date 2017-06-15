model led_blue_red
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
            digital_out := sComm.cmd_digital_out(1, 11, 1);// This will turn on the red LED
            sComm.delay(5000);// Delay for 5 seconds
            digital_out := sComm.cmd_digital_out(1, 9, 0);// This will turn off the blue LED
            sComm.delay(3000);// Delay for 3 seconds
            digital_out := sComm.cmd_digital_out(1, 11, 0);
          end if;// This will turn off the red LED
          c_ok := sComm.close_serial(1);// To close the connection safely
end led_blue_red;
