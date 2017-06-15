model ldr_read
          extends Modelica.Icons.Example;
          import sComm = SerialCommunication;
          import Exn = Modelica.Exceptions;
          import strm = Modelica.Utilities.Streams;
          //import str = Modelica.Reference.Operators;
          
          Integer ok(fixed = false);
          Integer analog_in(fixed = false);
          //Integer digital_out(fixed = false);
          Integer c_ok(fixed = false);
          //String str1(fixed = false);
          
          algorithm
            ok := sComm.open_serial(1, 2, 115200);// At port 0 with baudrate of 115200
            if ok <> 0 then
              strm.print("Unable to open serial port, please check");
            else
              for i in 1:20 loop//Run for 20 iterations
                analog_in := sComm.cmd_analog_in(1, 5);// read analog pin 5 (ldr)
                //str1 := str.string(analog_in);
                //strm.print(str1);
                //sComm.delay(500);
              end for;
            end if;
          c_ok := sComm.close_serial(1);// To close the connection safely
        end ldr_read;
