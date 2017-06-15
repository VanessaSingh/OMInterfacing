model push_button_status
          extends Modelica.Icons.Example;
          import sComm = SerialCommunication;
          import Exn = Modelica.Exceptions;
          import strm = Modelica.Utilities.Streams;
          Integer ok(fixed = false);
          Integer digital_in(fixed = false);
          Integer c_ok(fixed = false);
        algorithm
          ok := sComm.open_serial(1, 0, 115200);
// At port 0 with baudrate of 115200
          if ok <> 0 then
            strm.print("Unable to open serial port, please check");
          else
            for i in 1:1000 loop
              digital_in := sComm.cmd_digital_in(1, 12);
              sComm.cmd_arduino_meter(digital_in);
            end for;
          end if;
          c_ok := sComm.close_serial(1);
// To close the connection safely
        end push_button_status;
   
