model therm_read
          extends Modelica.Icons.Example;
          import sComm = SerialCommunication;
          import Exn = Modelica.Exceptions;
          import strm = Modelica.Utilities.Streams;
          
          Integer ok(fixed = false);
          Integer analog_in(fixed = false);
          //Integer digital_out(fixed = false);
          Integer c_ok(fixed = false);
          
          algorithm
            ok := sComm.open_serial(1, 2, 115200);// At port 0 with baudrate of 115200
            if ok <> 0 then
              strm.print("Unable to open serial port, please check");
            else
              for i in 1:20 loop//Run for 20 iterations
                analog_in := sComm.cmd_analog_in(1, 4);// read analog pin 5 (ldr)
                //strm.print(analog_in);
                sComm.delay(500);
              end for;
            end if;
          c_ok := sComm.close_serial(1);// To close the connection safely
        end therm_read;  
        
