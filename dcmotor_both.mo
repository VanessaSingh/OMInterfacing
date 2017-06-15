model dcmotor_both
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
            strm.print("Unable to open serial port, please check");
          else
            sComm.cmd_dcmotor_setup(1, 3, 1, 9, 10);
            sComm.cmd_dcmotor_run(1, 1, 100);
            sComm.delay(3000);
            sComm.cmd_dcmotor_run(1, 1, -100);
            sComm.delay(2000);
            sComm.cmd_dcmotor_release(1, 1);
          end if;
// Setup DC motor of type 3 (L293D), motor 1, pin 9 and 10
// Motor 1 runs at PWM 100
// for 3 seconds
// Motor 1 runs at PWM -100 in reverse direction
// for 2 seconds
// Motor 1 is released
          c_ok := sComm.close_serial(1);
// To close the connection safely
        end dcmotor_both;
