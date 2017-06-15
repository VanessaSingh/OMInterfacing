model servo_pot
          extends Modelica.Icons.Example;
          import sComm = SerialCommunication;
          import Exn = Modelica.Exceptions;
          import strm = Modelica.Utilities.Streams;
          //import fl = ModelicaReference.Operators;
          Integer ok(fixed = false);
          Integer c_ok(fixed = false);
          Integer analog_in(fixed = false);
          //Real p(fixed=false);
        algorithm
          ok := sComm.open_serial(1, 2, 115200);
//COM port is 0 and baud rate is 115200
          if ok <> 0 then
            strm.print("Check the serial port and try again");
          else
            sComm.cmd_servo_attach(1, 1);
            sComm.delay(500);
            for i in 1:5000 loop
              analog_in := sComm.cmd_analog_in(1, 2);
              analog_in := sComm.changetofloor(analog_in * (180 / 1023));
              sComm.cmd_servo_move(1, 1, analog_in);
              sComm.delay(500);
            end for;
            sComm.cmd_servo_detach(1, 1);
          end if;
// Attach the motor to pin 9
// 5,000 itterations
// Read potntiometer value
// Scale Potentiometer value to 0-180
// Command the servo motor
// sleep for 500 milliseconds
// Detach the motor
          c_ok := sComm.close_serial(1);
// To close the connection safely
        end servo_pot;
