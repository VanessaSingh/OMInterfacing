/**************************************************

file: main.c
purpose: serial librairie for Scilab

Alain Caignot 
**************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
//#ifdef _WIN32
#include <Windows.h>
#include <fcntl.h>
#include <math.h>
#include "serial.h"

#define abs(x) (x>=0?x:-x)
//#else
//#include <unistd.h>
//#endif

#define MAXPORTS 5 //unused : can be used to opened several COM to have several Arduino card

// Static definition to stock HANDLE of Port.
static HANDLE hport;

// Function to open port COM
__declspec (dllexport) __stdcall int open_serial(int handle,int port, int baudrate){
	DCB dcbSerialParams ;
	DWORD dwBytesWrite = 0;
	DWORD dwBytesRead = 10;
	COMMTIMEOUTS timeouts={0};

	char tmp[5]="COM5";
	itoa(port,&tmp[3],10);
	
	int OK=0;
	

  	hport = CreateFile(tmp,
							GENERIC_READ | GENERIC_WRITE,
							0,//FILE_SHARE_READ | FILE_SHARE_WRITE //to test : recuperation COM port if simulation crashes
							0,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							0);

	if(hport==INVALID_HANDLE_VALUE){
	  if(GetLastError()==ERROR_FILE_NOT_FOUND){
	     //serial port does not exist. Inform user.
	     OK = GetLastError();
		 return OK;
	  }
	//some other error occurred. Inform user.
	  OK = GetLastError();
	  return OK;
	}



	dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
	if (!GetCommState(hport, &dcbSerialParams)) {
	//error getting state
	     OK = GetLastError();
		 return OK;
	}
	dcbSerialParams.BaudRate=baudrate;
	dcbSerialParams.ByteSize=8;
	dcbSerialParams.StopBits=ONESTOPBIT;
	dcbSerialParams.Parity=NOPARITY;
	if(!SetCommState(hport, &dcbSerialParams)){
		//error setting serial port state
	    OK = GetLastError();
		return OK;
	}
    

	timeouts.ReadIntervalTimeout=50;
	timeouts.ReadTotalTimeoutConstant=50;
	timeouts.ReadTotalTimeoutMultiplier=1;
	timeouts.WriteTotalTimeoutConstant=50;
	timeouts.WriteTotalTimeoutMultiplier=1;
	if(!SetCommTimeouts(hport, &timeouts)){
	//error occureed. Inform user
		OK = GetLastError();
	    return OK;
	}
	Sleep(1000);
}


__declspec (dllexport) __stdcall int close_serial(int handle){
	int res;
	int OK=0;
	res=CloseHandle(hport);
	if (res==0)//error
		OK = GetLastError();
	return OK;
}

__declspec (dllexport) __stdcall int write_serial(int handle, char str[],int size){
	DWORD dwBytesWrite = 0;
	int res;
	int OK=0;
	res=WriteFile(hport,str,size,&dwBytesWrite,NULL);

	if (res==0) //error
		OK = GetLastError();

}

__declspec (dllexport) __stdcall int status_serial(int handle, int nb[]){
	DWORD dwErrorFlags;
	COMSTAT ComStat;
	int res;

	int OK=0;
	res=ClearCommError( hport, &dwErrorFlags, &ComStat );
	if (res==0) {//error
		OK = GetLastError();
        return OK;
	}
	nb[0] = 2; 
    nb[1] = 2;
    OK=0;
    //printf("Getting status.....\n");
    return OK;
}

__declspec (dllexport) __stdcall int read_serial(int handle,int buf[],int size){
	DWORD dwBytesRead = 0;
	int res;

	//*OK=0;
	res=ReadFile(hport, buf, size, &dwBytesRead, NULL);
	//if (res==0) {//error
	//	*OK = GetLastError();
        return res;
	//}
}
__declspec (dllexport) __stdcall int delay(long int t)
{ 
	sleep(abs(t/1000));
	return 0;
}

__declspec (dllexport) __stdcall int cmd_digital_out(int h,int pin_no,int val)
{   
	int wr=2;
	char pin[6]="Da";
    char v[2],temp[2];
	sprintf(temp,"%c",pin_no+48);
	strcat(pin,temp);
	strcat(pin,"1");
    printf("%s",pin);
  	wr=write_serial(h,pin,4);
  	if (val > 0.5)
    	val = 1;
  	else
    	val = 0;

	sprintf(v,"%d",val);
  	strcpy(pin,"Dw");
    strcat(pin,temp);
	strcat(pin,v);
    printf("%s",pin);
  	wr=write_serial(h,pin,4);
    return wr;
}

__declspec (dllexport) __stdcall int cmd_digital_in(int h,int pin_no)
{
    int value = 0;
        char pin[6]="Da";
        char v1[2],v2[2];
        int wr1, wr2;
        sprintf(v1,"%c",pin_no+48);
        strcat(pin,v1);
        strcat(pin,"0");
        //printf("%s\n",pin);
        wr1=write_serial(h,pin,4);

        strcpy(pin,"Dr");
        sprintf(v2,"%c",pin_no+48);
        strcat(pin,v2);
        wr2=write_serial(1,pin,3);
        //binary transfer
        int stat;
        int num_bytes[2];
        char st[10];
        stat=status_serial(h,num_bytes);
        while(num_bytes[0]<1)
            stat=status_serial(h,num_bytes);
        char* temp;
        int wr=read_serial(h,st,1);
        value=strtod(st,&temp);
        printf("%d\n",value);
        return value;
}
__declspec (dllexport) __stdcall unsigned int cmd_analog_in(int h,int pin_no)
{	
	char pin[5],v1[2];
	int stat;
	int read[2];
	sprintf(v1,"%c",pin_no+48);
	strcpy(pin,"A");
	strcat(pin,v1);
    int wr=write_serial(h,pin,2);
    stat=status_serial(h,read);
    while (read[0] < 2)
    	stat=status_serial(h,read);
    char values[5];
    int a_rd=read_serial(h,values,2);
    printf("%s\n",values);
    int l=strlen(values);
    int temp[l];
    int i;
    for (i = 0; i < l; ++i)
    {
    	temp[i]=(int)values[i];
    }
    unsigned int result=(int)(256*temp[1]+temp[0]);
    printf("%d\n",result);
    return result;
}

__declspec (dllexport) __stdcall int cmd_analog_out(int h,int pin_no,double val)
{
	char v1[2],v2[2];
	if(val > 255)
    	val = 255;
    else if(val < 0)
        val = 0;
    printf("%f\n",val);
    char code_sent[10];
    strcpy(code_sent,"W");
    sprintf(v1,"%c",pin_no+48);
    sprintf(v2,"%c",abs(ceil(val)));
    strcat(code_sent,v1);
    strcat(code_sent,v2);
    printf("%s\n",code_sent);
    return write_serial(h,code_sent,3);

}
/*__declspec (dllexport) __stdcall void cmd_dcmotor_setup(int h,int driver_type,int motor_no,int pin_no_1,int pin_no_2)
{
	printf("init DCmotor\n");
	char code_sent[10]="C";
	int wr;
	char v1[2],v2[2],motor[2];
    if(driver_type==1)
    {
    	sprintf(v1,"%c",pin_no_1+48);
    	sprintf(v2,"%c",pin_no_2+48);
    	sprintf(motor,"%d",motor_no);
    	strcat(code_sent,motor);
    	strcat(code_sent,v1);
    	strcat(code_sent,v2);
    	strcat(code_sent,"1");
    }
        //code_sent="C"+string(motor_no)+ascii(48+pin_no_1)+ascii(48+pin_no_2)+"1";    //adafruit
    else if(driver_type==2)
    {
    	sprintf(v1,"%c",pin_no_1+48);
    	sprintf(v2,"%c",pin_no_2+48);
    	sprintf(motor,"%d",motor_no);
    	strcat(code_sent,motor);
    	strcat(code_sent,v1);
    	strcat(code_sent,v2);
    	strcat(code_sent,"1");
    }    
        //code_sent="C"+string(motor_no)+ascii(48+pin_no_1)+ascii(48+pin_no_2)+"1";   //code pour initialiser L298
    else if(driver_type==3)
    {
    	sprintf(v1,"%c",pin_no_1+48);
    	sprintf(v2,"%c",pin_no_2+48);
    	sprintf(motor,"%d",motor_no);
    	strcat(code_sent,motor);
    	strcat(code_sent,v1);
    	strcat(code_sent,v2);
    	strcat(code_sent,"0");

    }
    //    code_sent="C"+string(motor_no)+ascii(48+pin_no_1)+ascii(48+pin_no_2)+"0";   //code pour initialiser L293

    wr=write_serial(h,code_sent,5);

    int stat;
    int num_bytes[2];
    //Attente que l'arduino reponde OK
    stat=status_serial(1,num_bytes);
    while (num_bytes[0] < 2) 
        stat=status_serial(1,num_bytes);
    char values[5];
    int dcm_rd=read_serial(1,values,2);
    if (dcm_rd == 0)
        printf("Init motor successful\n");
    else
        printf("Init motor unsuccessful\n");
    //return dcm_rd;
}

__declspec (dllexport) __stdcall void cmd_dcmotor_run(int h,int motor_no,int u1)
{
	char code_dir[2];
	char code_sent[10]="W";
	char motor[2];
    int val;
	char v[2];
    if(u1>=0)
       	sprintf(code_dir,"%c",49);
              //code_dir=ascii(49);
    else
        sprintf(code_dir,"%c",48);
              //code_dir=ascii(48);
    if(abs(u1)>255)
        val=255;
    else
        val=abs(ceil(u1));
    sprintf(motor,"%c",motor_no+48);
    sprintf(v,"%c",val+48);
    strcat(code_sent,motor);
    strcat(code_sent,code_dir);
    strcat(code_sent,v);      
          //code_sent="M"+ascii(48+motor_no)+code_dir+ascii(val);
    int wr=write_serial(h,code_sent,4);
    //return wr;
}

__declspec (dllexport) __stdcall void cmd_dcmotor_release(int h,int motor_no)
{
	char code_sent[10]="M";
	char motor[2];
	char v[2];
	sprintf(motor,"%d",motor_no);
	sprintf(v,"%c",0+48);
    strcat(code_sent,motor);
    strcat(code_sent,"1");
    strcat(code_sent,v);
	//code_sent="M"+ascii(48+motor_no)+"1"+ascii(0);
    int wr=write_serial(h,code_sent,4);
    strcpy(code_sent,"M");  
    strcat(code_sent,motor);
    strcat(code_sent,"r");
    //code_sent="M"+ascii(48+motor_no)+"r";
    wr=write_serial(h,code_sent,3);
    //return wr;
}*/
__declspec (dllexport) __stdcall void cmd_servo_attach(int h,int servo_no)
{
	char pin[5];
	int wr;
	printf("init servo write");
    if(servo_no==1)//servo 1 on pin 9
    {
    	strcpy(pin,"Sa1");
        wr=write_serial(h,pin,3);
    }     
    else if(servo_no==2)//servo 2 on pin 10
    {
    	strcpy(pin,"Sa2");
        wr=write_serial(h,pin,3);
    }
    else
           perror("Error.");
}


__declspec (dllexport) __stdcall void cmd_servo_move(int h,int servo_no,int u1)
{
	int wr;
	char servo[2];
	char v[2];
	char pin[5]="Sw";
	if (u1<0)
	{
		sprintf(servo,"%c",48+servo_no);
		sprintf(v,"%c",48+0);
		strcat(pin,servo);
		strcat(pin,v);
          //pin="Sw"+ascii(48+servo_no)+ascii(0);
	}
    else if(u1>180)
    {
    	sprintf(servo,"%c",48+servo_no);
		sprintf(v,"%c",48+180);
		strcat(pin,servo);
		strcat(pin,v);
    }      //pin="Sw"+ascii(48+servo_no)+ascii(180);
    else
    {
    	sprintf(servo,"%c",48+servo_no);
		sprintf(v,"%c",48+(unsigned int)u1);
		strcat(pin,servo);
		strcat(pin,v);
    }      //pin="Sw"+ascii(48+servo_no)+ascii(uint8(u1));
    wr=write_serial(1,pin,4);
}


__declspec (dllexport) __stdcall void cmd_servo_detach(int h,int servo_no)
{
	int wr;
	char pin[5]="Sd";
	if(servo_no==1) //servo 1 on pin 10
    {
    	strcat(pin,"1");
           //pin="Sd1"
        wr=write_serial(h,pin,3);
    }
    else if(servo_no==2) //servo 2 on pin 9
    {
    	strcat(pin,"2");
           //pin="Sd2"
        wr=write_serial(h,pin,3);
    }
    else
    perror("Error.");

}
__declspec (dllexport) __stdcall int changetofloor(float num)
{
		int n = floor(num)	;
		return n;
}

void cmd_dcmotor_setup(int h,int driver_type,int motor_no,int pin_no_1,int pin_no_2)
{
    printf("init DCmotor\n");
    char code_sent[10]="C";
    int wr;
    char v1[2],v2[2],motor[2];
    if(driver_type==1)
    {
        sprintf(v1,"%c",pin_no_1+48);
        sprintf(v2,"%c",pin_no_2+48);
        sprintf(motor,"%c",motor_no+48);
        strcat(code_sent,motor);
        strcat(code_sent,v1);
        strcat(code_sent,v2);
        strcat(code_sent,"1");
    }
        //code_sent="C"+string(motor_no)+ascii(48+pin_no_1)+ascii(48+pin_no_2)+"1";    //adafruit
    else if(driver_type==2)
    {
        sprintf(v1,"%c",pin_no_1+48);
        sprintf(v2,"%c",pin_no_2+48);
        sprintf(motor,"%c",motor_no+48);
        strcat(code_sent,motor);
        strcat(code_sent,v1);
        strcat(code_sent,v2);
        strcat(code_sent,"1");
    }    
        //code_sent="C"+string(motor_no)+ascii(48+pin_no_1)+ascii(48+pin_no_2)+"1";   //code pour initialiser L298
    else if(driver_type==3)
    {
        sprintf(v1,"%c",pin_no_1+48);
        sprintf(v2,"%c",pin_no_2+48);
        sprintf(motor,"%c",motor_no+48);
        strcat(code_sent,motor);
        strcat(code_sent,v1);
        strcat(code_sent,v2);
        strcat(code_sent,"0");

    }
    //    code_sent="C"+string(motor_no)+ascii(48+pin_no_1)+ascii(48+pin_no_2)+"0";   //code pour initialiser L293
    printf("%s\n",code_sent);
    wr=write_serial(h,code_sent,5);

    int stat;
    int num_bytes[2];
    //Attente que l'arduino reponde OK
    stat=status_serial(1,num_bytes);
    while (num_bytes[0] < 2) 
        stat=status_serial(1,num_bytes);
    char values[5];
    int dcm_rd=read_serial(1,values,2);
    printf("%d\n",dcm_rd );
    printf("%s\n",values );
    if (dcm_rd == 0)
        printf("Init motor successful\n");
    else
        printf("Init motor unsuccessful\n");
}

void cmd_dcmotor_run(int h,int motor_no,int u1)
{
    char code_dir[2];
    char code_sent[10]="M";
    char motor[2];
    int val;
    char v[2];
    if(u1>=0)
        sprintf(code_dir,"%c",49);
              //code_dir=ascii(49);
    else
        sprintf(code_dir,"%c",48);
              //code_dir=ascii(48);
    if(abs(u1)>255)
        val=255;
    else
        val=abs(ceil(u1));
    printf("%d\n",val);
    sprintf(motor,"%c",motor_no+48);
    sprintf(v,"%c",val);
    strcat(code_sent,motor);
    strcat(code_sent,code_dir);
    strcat(code_sent,v);
    printf("%s\n",code_sent);      
          //code_sent="M"+ascii(48+motor_no)+code_dir+ascii(val);
    int wr=write_serial(h,code_sent,4);
}

void cmd_dcmotor_release(int h,int motor_no)
{
    char code_sent[6]="M";
    char motor[2];
    char v[2];
    sprintf(motor,"%c",motor_no+48);
    sprintf(v,"%c",0);
    strcat(code_sent,motor);
    strcat(code_sent,"1");
    strcat(code_sent,v);
    //code_sent="M"+ascii(48+motor_no)+"1"+ascii(0);
    printf("%s\n",code_sent);
    int wr=write_serial(h,code_sent,4);
    strcpy(code_sent,"M");  
    strcat(code_sent,motor);
    strcat(code_sent,"r");
    printf("%s\n",code_sent);
    //code_sent="M"+ascii(48+motor_no)+"r";
    wr=write_serial(h,code_sent,3);
}
/*__declspec (dllexport) __stdcall long int hex2dec(char hexadecimal[])
{
    long int decimalNumber=0;
    char hexDigits[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
      '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    int i, j, power=0;    
     
    /* Converting hexadecimal number to decimal number */
    for(i=strlen(hexadecimal)-1; i >= 0; i--) {
        /*search currect character in hexDigits array */
        if(isalpha(hexadecimal[i]))
                hexadecimal[i]=toupper(hexadecimal[i]);
        for(j=0; j<16; j++){
            if(hexadecimal[i] == hexDigits[j]){
                decimalNumber += j*pow(16, power);
            }
        }
        power++;
    }
    return decimalNumber;
}
/*
__declspec (dllexport) __stdcall char *dec2hex(long int decimalnum)
{
    long quotient, remainder;
    int j = 0;
    char hexadecimalnum[100];
 
    quotient = decimalnum;
 
    while (quotient != 0)
    {
        remainder = quotient % 16;
        if (remainder < 10)
            hexadecimalnum[j++] = 48 + remainder;
        else
            hexadecimalnum[j++] = 55 + remainder;
        quotient = quotient / 16;
    }
    return hexadecimalnum;
}

__declspec (dllexport) __stdcall double ieeesingle2num(char hexa[])
{
    long int x=hex2dec(hexa);
    long int k =(long int)pow(2,31);   //pow2(31);
    int s = (x >= k);
    if (s) 
        x = x - k;
    //t = pow2(x,-23);
    double t = x*pow(2,-23);
    long int e = floor(t);
    double f = t - e;
    double y;
    if (e == 255)
        if(f == 0)
            y = INFINITY; 
        else 
            y = NAN;
    else if(e > 0)
        y = (1+f)*pow(2,e-127);
     //y = (1+f).*2.^(e-127);
 
    else
    y = f*pow(2,-126);
    //y = f.*2.^-126;
    if (s) 
        y = -y;
    return y;
}

__declspec (dllexport) __stdcall char ascii_n(int num)
{
    return (char)num;
}

__declspec (dllexport) __stdcall int ascii_c(char c)
{
    return (int)c;
}

__declspec (dllexport) __stdcall double read_val(int addr_byte)
{   
    int h=open_serial(1,0,9600);
    char array1[20];
    if(addr_byte==86)
    {   
        char arr[8]={ascii_n(1),ascii_n(3),ascii_n(15),ascii_n(86),ascii_n(00),ascii_n(2),ascii_n(39),ascii_n(15)};
        int j;
        for (j = 0; j < 8; ++j)
        {
            strcat(array1,arr[j]);
        }
        printf("Voltage(in V)=");
    }
    else if(addr_byte==88)
    {   
        char arr[8]={ascii_n(1),ascii_n(3),ascii_n(15),ascii_n(88),ascii_n(00),ascii_n(2),ascii_n(70),ascii_n(204)};
        int j;
        for (j = 0; j < 8; ++j)
        {
            strcat(array1,arr[j]);
        }
        printf("Current(in A)=");
    }
    else if(addr_byte==78)
    {   
        char arr[8]={ascii_n(1),ascii_n(3),ascii_n(15),ascii_n(78),ascii_n(00),ascii_n(2),ascii_n(167),ascii_n(8)};
        int j;
        for (j = 0; j < 8; ++j)
        {
            strcat(array1,arr[j]);
        }
        printf("Active Power(in W)=");
    }

    int wr=write_serial(1,array1,8);
    char buf[12];
    int rd=read_serial(1,buf,11);
    int b1=0,b2=0,b3=0,b4=0;
    int myresult[12];
    int i;
    for (i = 0; i < 11; ++i)
    {
        myresult[i]=ascii_c(buf[i]);
    }
    
    int a1=myresult[5];
    if (a1<16)
    {
        b1=1;
    }
    char* v1=dec2hex(a1);
    if (b1)
    {
        strcat("0",v1);
    }

    int a2=myresult[6];
    if (a2<16)
    {
        b2=1;
    }
    char* v2=dec2hex(a2);
    if (b2)
    {
        strcat("0",v2);
    }

    int a3=myresult[7];
    if (a3<16)
    {
        b3=1;
    }
    char* v3=dec2hex(a3);
    if (b3)
    {
        strcat("0",v3);
    }

    int a4=myresult[8];
    if (a4<16)
    {
        b4=1;
    }
    char* v4=dec2hex(a4);
    if (b4)
    {
        strcat("0",v4);
    }
    char* a5[4]={v3,v4,v1,v2};
    char a6[100];
    for (i = 0; i < 4; ++i)
    {
        strcat(a6,a5[i]);   
    }
    int cl=close_serial(1);
    double p=ieeesingle2num(a6);
    printf("%f",p);
    return p;
}

__declspec (dllexport) __stdcall void read_voltage()
{
    int h=open_serial(1,0,9600);
    char acc[10];
    char arr[8]={ascii_n(1),ascii_n(3),ascii_n(15),ascii_n(86),ascii_n(00),ascii_n(2),ascii_n(39),ascii_n(15)};
    int j;
    for (j = 0; j < 8; ++j)
    {
        strcat(acc,arr[j]);
    }
    //printf("%s\n",acc );
    int wr=write_serial(1,arr,8);
    char buf[12];
    int rd = read_serial(1,buf,11);
    int b1=0,b2=0,b3=0,b4=0;
    int myresult[12];
    int i;
    for (i = 0; i < 11; ++i)
    {
        myresult[i]=ascii_c(buf[i]);
    }
    
    int a1=myresult[5];
    if (a1<16)
    {
        b1=1;
    }
    char* v1=dec2hex(a1);
    if (b1)
    {
        strcat("0",v1);
    }

    int a2=myresult[6];
    if (a2<16)
    {
        b2=1;
    }
    char* v2=dec2hex(a2);
    if (b2)
    {
        strcat("0",v2);
    }

    int a3=myresult[7];
    if (a3<16)
    {
        b3=1;
    }
    char* v3=dec2hex(a3);
    if (b3)
    {
        strcat("0",v3);
    }

    int a4=myresult[8];
    if (a4<16)
    {
        b4=1;
    }
    char* v4=dec2hex(a4);
    if (b4)
    {
        strcat("0",v4);
    }
    char* a5[4]={v3,v4,v1,v2};
    char a6[100];
    for (i = 0; i < 4; ++i)
    {
        strcat(a6,a5[i]);   
    }
    int cl=close_serial(1);
    double p=ieeesingle2num(a6);
    printf("Voltage(in V)=%f",p);
}


__declspec (dllexport) __stdcall void read_current()
{
    int h=open_serial(1,0,9600);
    char acc[100];
    char arr[8]={ascii_n(1),ascii_n(3),ascii_n(15),ascii_n(88),ascii_n(00),ascii_n(2),ascii_n(70),ascii_n(204)};
    int j;
    for (j = 0; j < 8; ++j)
    {
        strcat(acc,arr[j]);
    }
        //printf("%s\n",acc );
    int wr=write_serial(1,arr,8);
    char buf[12];
    int rd=read_serial(1,buf,11);
    int b1=0,b2=0,b3=0,b4=0;
    int myresult[12];
    int i;
    for (i = 0; i < 11; ++i)
    {
        myresult[i]=ascii_c(buf[i]);
    }

    int a1=myresult[5];
    if (a1<16)
    {
        b1=1;
    }
    char* v1=dec2hex(a1);
    if (b1)
    {
        strcat("0",v1);
    }

    int a2=myresult[6];
    if (a2<16)
    {
        b2=1;
    }
    char* v2=dec2hex(a2);
    if (b2)
    {
        strcat("0",v2);
    }

    int a3=myresult[7];
    if (a3<16)
    {
        b3=1;
    }
    char* v3=dec2hex(a3);
    if (b3)
    {
        strcat("0",v3);
    }

    int a4=myresult[8];
    if (a4<16)
    {
        b4=1;
    }
    char* v4=dec2hex(a4);
    if (b4)
    {
        strcat("0",v4);
    }
    char* a5[4]={v3,v4,v1,v2};
    char a6[100];
    for (i = 0; i < 4; ++i)
    {
        strcat(a6,a5[i]);   
    }
    int cl=close_serial(1);
    double p=ieeesingle2num(a6);
    printf("Current(in A)=%f",p);
}

__declspec (dllexport) __stdcall void read_active_power()
{
    int h=open_serial(1,0,9600);
    char acc[100];
    char arr[8]={ascii_n(1),ascii_n(3),ascii_n(15),ascii_n(78),ascii_n(00),ascii_n(2),ascii_n(167),ascii_n(8)};
    int j;
    for (j = 0; j < 8; ++j)
    {
        strcat(acc,arr[j]);
    }
        //printf("%s\n",acc );
    int wr=write_serial(1,arr,8);
    char buf[12];
    int rd=read_serial(1,buf,11);
    int b1=0,b2=0,b3=0,b4=0;
    int myresult[12];
    int i;
    for (i = 0; i < 11; ++i)
    {
        myresult[i]=ascii_c(buf[i]);
    }
    
    int a1=myresult[5];
    if (a1<16)
    {
        b1=1;
    }
    char* v1=dec2hex(a1);
    if (b1)
    {
        strcat("0",v1);
    }

    int a2=myresult[6];
    if (a2<16)
    {
        b2=1;
    }
    char* v2=dec2hex(a2);
    if (b2)
    {
        strcat("0",v2);
    }

    int a3=myresult[7];
    if (a3<16)
    {
        b3=1;
    }
    char* v3=dec2hex(a3);
    if (b3)
    {
        strcat("0",v3);
    }

    int a4=myresult[8];
    if (a4<16)
    {
        b4=1;
    }
    char* v4=dec2hex(a4);
    if (b4)
    {
        strcat("0",v4);
    }
    char* a5[4]={v3,v4,v1,v2};
    char a6[100];
    for (i = 0; i < 4; ++i)
    {
        strcat(a6,a5[i]);   
    }
    int cl=close_serial(1);
    double p=ieeesingle2num(a6);
    printf("Active Power(in W)=%f",p);
}
*/