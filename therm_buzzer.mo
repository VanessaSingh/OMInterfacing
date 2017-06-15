model therm_buzzer
          extends Modelica.Icons.Example;
          import sComm = SerialCommunication;
          import Exn = Modelica.Exceptions;
          import strm = Modelica.Utilities.Streams;
          
          Integer ok(fixed = false);
          Integer analog_in(fixed = false);
          Integer digital_out(fixed = false);
          Integer c_ok(fixed = false);
          
          algorithm
            ok := sComm.open_serial(1, 0, 115200);// At port 0 with baudrate of 115200
            if ok <> 0 then
              strm.print("Unable to open serial port, please check");
            else
              for i in 1:500 loop//Run for 500 iterations
                analog_in := sComm.cmd_analog_in(1, 4);// read analog pin 4
                if analog_in > 500 then// Setting Threshold value of 300
                  digital_out := sComm.cmd_digital_out(1, 3, 1);// Turn ON LED
                else
                  digital_out := sComm.cmd_digital_out(1, 3, 0);// Turn OFF LED
                end if;
                sComm.delay(500);
              end for;
            end if;
          c_ok := sComm.close_serial(1);// To close the connection safely
        end therm_buzzer;
