model SerialCommunication

  function open_serial
    input Integer handle, port, baudrate;
    output Integer OK;
    external OK=open_serial(handle, port, baudrate) annotation(
      Library = "D:/interfacing/SerialComm.dll",
      LibraryDirectory = "D:/interfacing",
      Include = "#include \"D:/interfacing/serial.h\"");
  end open_serial;
  
  function write_serial
    input Integer handle ;
    input String str ;
    input Integer size;
    output Integer OK;
    external OK=write_serial(handle,str,size) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
  end write_serial;    
  
  function read_serial
    input Integer handle;    
    input Integer size;
    output Integer OK;
    protected Integer buf[size+1];
    external OK=read_serial(handle,buf,size) annotation(
      Library = "D:/interfacing/SerialComm.dll",
      LibraryDirectory = "D:/interfacing",
      Include = "#include \"D:/interfacing/serial.h\"");
  end read_serial;

  
  function status_serial
    input Integer handle;
    output Integer stat_OK;
    protected Integer bytes[2];
    external stat_OK=status_serial(handle,bytes) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
  end status_serial;
  
  function close_serial
    input Integer handle;
    output Integer c_OK;
    external c_OK=close_serial(handle) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
  end close_serial;
  
  function cmd_digital_in
    input Integer h,pin_no;
    output Integer digital_in;
    external digital_in=cmd_digital_in(h,pin_no) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
  end cmd_digital_in;
  
  function cmd_digital_out
    input Integer h,pin_no,val;
    output Integer digital_w_OK;
    external digital_w_OK=cmd_digital_out(h,pin_no,val) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
  end cmd_digital_out;
  
  function delay
    input Integer t;
    //output Integer delay_OK;
    external delay(t) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
  end delay;

  
  function cmd_analog_in
    input Integer h,pin_no;
    output Integer val;
    external val=cmd_analog_in(h,pin_no) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
  end cmd_analog_in;


  function cmd_analog_out
    input Integer h,pin_no;
    input Real val;
    output Integer analog_w_OK;
    external analog_w_OK=cmd_analog_out(h,pin_no,val) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
  end cmd_analog_out;
  
      function cmd_dcmotor_setup
      extends Modelica.Icons.Function;
      input Integer handle,driver_type,motor_no,pin1,pin2;
      //output Integer dcmotor_setup_OK;
        external cmd_dcmotor_setup(handle,driver_type,motor_no,pin1,pin2) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
    end cmd_dcmotor_setup;
    
    function cmd_dcmotor_run
      extends Modelica.Icons.Function;
      input Integer handle,motor_no,val;
      //output Integer dcmotor_run_OK;
        external cmd_dcmotor_run(handle,motor_no,val) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
    end cmd_dcmotor_run;
    
    function cmd_dcmotor_release
      extends Modelica.Icons.Function;
      input Integer handle,motor_no;
      //output Integer dcmotor_run_OK;
        external cmd_dcmotor_release(handle,motor_no) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
    end cmd_dcmotor_release;

    function cmd_servo_attach
      extends Modelica.Icons.Function;
      input Integer handle,servo_no;
      //output Integer dcmotor_release_OK;
        external cmd_servo_attach(handle,servo_no) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
    end cmd_servo_attach;
    
    function cmd_servo_move
      extends Modelica.Icons.Function;
      input Integer handle,servo_no,u1;
      //output Integer dcmotor_release_OK;
        external cmd_servo_move(handle,servo_no,u1) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
    end cmd_servo_move;
    
     function cmd_servo_detach
      extends Modelica.Icons.Function;
      input Integer handle,servo_no;
      //output Integer dcmotor_release_OK;
        external cmd_servo_detach(handle,servo_no) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
    end cmd_servo_detach;
    
    function changetofloor
      extends Modelica.Icons.Function;
      input Real num;
      output Integer n;
      //output Integer dcmotor_release_OK;
        external n=changetofloor(num) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
    end changetofloor;   
    
    function ieeesingle2num
      extends Modelica.Icons.Function;
      input String hexa;
      output Real n;
      //output Integer dcmotor_release_OK;
        external n=ieeesingle2num(hexa) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
    end ieeesingle2num; 
    
      function read_active_power
      extends Modelica.Icons.Function;
      //output Integer dcmotor_release_OK;
        external read_active_power() annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
    end read_active_power;
    
      function read_current
      extends Modelica.Icons.Function;
      //output Integer dcmotor_release_OK;
        external read_current() annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
    end read_current;
    
      function read_voltage
      extends Modelica.Icons.Function;
      //output Integer dcmotor_release_OK;
        external read_voltage() annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
    end read_voltage;
    
      function read_val
      extends Modelica.Icons.Function;
      input Integer a;
      output Real value;
      //output Integer dcmotor_release_OK;
        external value=read_val(a) annotation(Library="D:/interfacing/SerialComm.dll", LibraryDirectory="D:/interfacing", Include="#include \"D:/interfacing/serial.h\"");
    end read_val;
 
  Integer open_OK(fixed = false), write_OK(fixed=false), read_OK(fixed=false), close_ok(fixed=false); //dig_in(fixed=false), dig_out(fixed=false);
    equation
      open_OK = open_serial(1, 2, 99600);
      write_OK = write_serial(1, "v", 1);
      read_OK = read_serial(1,3);
      //status_OK = status_serial(1);
      //dig_in = cmd_digital_in(1, 9);
      //dig_out = cmd_digital_out(1, 10, 5);
      close_ok = close_serial(1);
      
 
  
end SerialCommunication;
