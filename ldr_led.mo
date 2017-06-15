model ldr_led
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
              for i in 1:500 loop//Run for 500 iterations
                analog_in := sComm.cmd_analog_in(1, 5);// read analog pin 5 (ldr)
                if analog_in < 300 then// Setting Threshold value of 300
                  digital_out := sComm.cmd_digital_out(1, 9, 1);// Turn ON LED
                else
                  digital_out := sComm.cmd_digital_out(1, 9, 0);// Turn OFF LED
                end if;
              end for;
            end if;
          c_ok := sComm.close_serial(1);// To close the connection safely
        end ldr_led;
