model led_blink "This will turn on and turn off the user LED for every second for 10 times"
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
            for i in 1:1000 loop
              digital_out := sComm.cmd_digital_out(1, 9, 0);// This will turn off the LED
              sComm.delay(1000);// Delay for 0.5 seconds
              digital_out := sComm.cmd_digital_out(1, 9, 1);// This will turn on the LED
              sComm.delay(1000);// Delay for 0.5 seconds
            end for;
          end if;
          c_ok := sComm.close_serial(1);// To close the connection safely
end led_blink;
