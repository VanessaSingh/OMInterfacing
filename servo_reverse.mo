model servo_reverse
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
            sComm.cmd_servo_move(1, 1, 90);
            sComm.delay(1000);
            sComm.cmd_servo_move(1, 1, 45);
            sComm.delay(1000);
            sComm.cmd_servo_detach(1, 1);
            sComm.delay(1000);
          end if;
// Attach the motor to pin 9. 1 means 9
// Move the servo to 90 degree
// be there for one second
// Move the servo to 45 degree
// be there for one second
// Detach the motor
          c_ok := sComm.close_serial(1);
// To close the connection safely
        end servo_reverse;
