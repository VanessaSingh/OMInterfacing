model led_green_blink "This will turn on and turn off the green LED for every second for 5 times"
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
            for i in 1:5 loop
              digital_out := sComm.cmd_digital_out(1, 10, 0);// This will turn off the green LED
              sComm.delay(1000);// Delay for 1 second
              digital_out := sComm.cmd_digital_out(1, 10, 1);// This turns on the green LED
              sComm.delay(1000);// Delay for 1 second
            end for;
          end if;
          c_ok := sComm.close_serial(1);// To close the connection safely
        end led_green_blink;
