model servo_loop
          extends Modelica.Icons.Example;
          import sComm = SerialCommunication;
          import Exn = Modelica.Exceptions;
          import strm = Modelica.Utilities.Streams;
          Integer ok(fixed = false);
          Integer c_ok(fixed = false);
          Integer angle(fixed = true);
        algorithm
          ok := sComm.open_serial(1, 2, 115200);
//COM port is 0 and baud rate is 115200
          if ok <> 0 then
            strm.print("Check the serial port and try again");
          else
            sComm.cmd_servo_attach(1, 1);
            sComm.delay(2000);
            angle := 20;
            for i in 1:10 loop
              sComm.cmd_servo_move(1, 1, angle * i);
              sComm.delay(2000);
            end for;
            sComm.cmd_servo_detach(1, 1);
          end if;
// Attach motor to pin 9. 1 means pin 9.
// Angle by which it has to move
// tell servo to rotate by 20 degrees
// waits for a sec
// Detach the motor
          c_ok := sComm.close_serial(1);
// To close the connection safely
        end servo_loop;
