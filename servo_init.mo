        model servo_init
          extends Modelica.Icons.Example;
          import sComm = SerialCommunication;
          import Exn = Modelica.Exceptions;
          import strm = Modelica.Utilities.Streams;
          Integer ok(fixed = false);
          Integer c_ok(fixed = false);
        algorithm
          ok := sComm.open_serial(1, 2, 115200);
//COM port is 0 and baud rate is 115200
          if ok <> 0 then
            strm.print("Check the serial port and try again");
          else
            sComm.cmd_servo_attach(1, 1);
            sComm.cmd_servo_move(1, 1, 30);
            //sComm.delay(3000);
          end if;
// To attach the motor to pin 9
// tell servo to rotate by 30 degrees
          c_ok := sComm.close_serial(1);
// To close the connection safely
        end servo_init;
