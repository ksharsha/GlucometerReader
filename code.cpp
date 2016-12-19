#include "mbed.h"

DigitalOut myled(LED1);
DigitalOut myled1(LED2);
DigitalOut myled2(LED3);
DigitalOut myled3(LED4);

AnalogIn ldr1(p15);
AnalogIn ldr2(p16);
AnalogIn ldr3(p17);

DigitalOut x(p18);
DigitalOut y(p19);
DigitalOut z(p20);

Serial txt(USBTX, USBRX); // tx, rx
//txt.baud(9600);

LocalFileSystem local("local");
float sample[3],sample_p[3],mean[3];
float sample_int[3],arr0[800],arr1[800],arr2[800],arr_m0[800],arr_m1[800],arr_m2[800],arrm0[800],arrm1[800],arrm2[800];
char control='c';
int flag=0,flag_s=0,i=0,j=15;  

int digit0=123;//first digit
int digit1=123;//second digit
int digit2=123;//third digit
int N=0;
int count=0;
int k=0;
int l=0;
int m=0;
int n1=0;
int n2=0;
int n3=0;//n1,n2,n3 represent the number of peaks and depressions finally it can at max be 15.
int num0[15],num1[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int digi_01[20],digi_11[20],digi_21[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int t0=150;
int t1=180;
int t2=100;
int count0,count1,count2=0;
int flag1=0;
int meand0,meand1,meand2=0;

void callback(){
    control = txt.getc();
    myled1 = !myled1;
    
    }

int main() {
   
    txt.attach(&callback);
    
    txt.printf("Hello World\r\n");
    
    while(1)
    {
       if(control == 'a')
        {
            break;
        } 
        myled3 = !myled3;
        
        wait(0.3);  
    }
    
    myled2 = 1;
    
    txt.printf("received a\r\n");
    
    txt.printf("Opening File...\r\n"); // Drive should be marked as removed
    FILE *fp = fopen("/local/samples.csv", "w");
    if(!fp) {
        fprintf(stderr, "File /local/samples.txt could not be opened!\n");
        exit(1);
    }
    
     
    while(1) {
        i=i+1;
    
    
        myled = 1;
    
        sample[0] = ldr1;
        sample[1] = ldr2;
        sample[2] = ldr3;
        arr0[i]=10000*sample[0];
        arr1[i]=10000*sample[1];
        arr2[i]=10000*sample[2];
        
        
        
        //sample[0] = sample[0] + 0.02;
        
        if(sample[0]<=0.50)
        {
            sample[0] = 0;
        }
        
        
        if(sample[1]<=0.50)
        {
            sample[1] = 0;
        }
 
       ////////////
        
        if(sample[2]<=0.50)
        {
            sample[2] = 0;
        }      
               fprintf(fp," %f, %f, %f \r\n", 10000*sample[0],10000*sample[1],10000*sample[2]);        
                       
                count++; 
        wait_ms(24);   
        
        myled =0;
        
        wait_ms(24);
        txt.printf("count equals %d\r\n",count);
        
        if(control == 'b')
        {
            break;
        }
    }
    fclose(fp);
    N=count;
    txt.printf("number of samples equals %d \r\n",N);
    txt.printf("broke the while loop\r\n");
    
    //glitch removal
    for (i=1;i<=N;i++)
    {
        if (arr0[i] < 8000.00) 
        {
            arr0[i] = arr0[i-1];
        }
    }
    for (i=1;i<=N;i++)
    {
        if (arr1[i] < 8500.00) 
        {    
            arr1[i] = arr1[i-1];
        }
    }
    for (i=1;i<=N;i++)
    {
        if (arr2[i] < 7000.00)
        { 
            arr2[i] = arr2[i-1];
        }
    }        
    
    //calculating means for different ldr readings
    //initializing means
    mean[0]=arr0[0];
     mean[1]=arr1[0];
      mean[2]=arr2[0];
      
    for (i=1;i<=N;i++)
    {
        mean[0]=arr0[i]+mean[0];
    }
    mean[0]=mean[0]/N;
    for (i=1;i<=N;i++)
    {
        mean[1]=arr1[i]+mean[1];
    }
    mean[1]=mean[1]/N;
    for (i=0;i<=N;i++)
    {
        mean[2]=arr2[i]+mean[2];
    }
    mean[2]=mean[2]/N;
    //subtracting mean from the signals
    txt.printf("calculating means done %f %f %f\r\n",mean[0],mean[1],mean[2]);
   for (i=1;i<N;i++)
    {
        
        arrm0[i]=arr0[i]-mean[0];
        txt.printf("arr0 %f %f\r\n",arr0[i],arrm0[i]);//correct
    }
     for (i=0;i<N;i++)
    {
        arrm1[i]=arr1[i]-mean[1];
    }
     for (i=0;i<N;i++)
    {
        arrm2[i]=arr2[i]-mean[2];
    }
    //checking if the new array is greater than or lesser than 0 and assigning 1 or -1 accordingly.
    //error here!!
    for (i=0;i<N;i++)
    {
        
        if(arrm0[i]>=t0)
        {
            arr_m0[i]=2.00;
        }
        else if(arrm0[i]>=-20.00 && arrm0[i]<=t0)
        {
            arr_m0[i]=1.00;
        }
        else if(arrm0[i]<-20.00)
        {
        arr_m0[i]=-1.00;
        }
        txt.printf("arr0 %f %f\r\n",arrm0[i],arr_m0[i]);//printing arrays of ones and minus ones
    }
    //threshhold for second ldr
     for (i=0;i<N;i++)
    {
        if(arrm1[i]>=t1)
        {
            arr_m1[i]=2.00;
        }
        else if(arrm1[i]>=50.00 && arrm1[i]<=t1)
        {
            arr_m1[i]=1;
        }
        else if(arrm1[i]<50.00)
        {
        arr_m1[i]=-1.00;
        }
        
    }
    //threshhold for third ldr
     for (i=0;i<N;i++)
    {
        
        if(arrm2[i]>=0.00)//this threshold is important
        {
            arr_m2[i]=1.00;
        }
        
        else if(arrm2[i]<0.00)
        {
        arr_m2[i]=-1.00;
        }
        
    }
    txt.printf("identifying depressions\r\n");
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //identifying depressions
    //z[k] gives the integer values where the three signals have depressions
    j=0;k=0;l=0;
    for(j=0;j<N;j++)
    {
        if(arr_m0[j]==-1)
        {
            if(arr_m1[j]==-1)
            {
                if(arr_m2[j]==-1)
                { 
                    num0[k]=j;
                    if( flag1==0)
                    {
                        num1[l]=j;
                        l=l+1;
                        flag1=1;
                    }
                    
                    txt.printf("%dth depresion\r\n",k);
                    if((arr_m2[j+1]!=-1)||(arr_m1[j+1]!=-1)||(arr_m0[j+1]!=-1))
                    {
                       k=k+1; 
                       flag1=0;
                      
                    }
                }
            }
        }
        
        
        
    }
    k=0;
    txt.printf("done ientifying depressions\r\n");
    txt.printf("indices of deression %d %d %d\r\n",num0[0],num0[1],num0[2]);
    //z[1],z[2],z[3] represent the ending oints of the three signals.
    //ended assigning 1 and -1 and 2 to the arrays.
    //counting and storing total number of peaks and depressions in the three signals
    
    //identifying what values are present in the first second and third signals till num0[1]
    digi_01[0]=arr_m0[num0[0]];
    digi_11[0]=arr_m1[num0[0]];
    digi_21[0]=arr_m2[num0[0]];
    k=0;l=0;m=0;
    i=num0[0];
    while(i<=num0[1])//replaced for loop with while loop
    {
        
        i++;
        
       // storing the sequence of peaks of the first digit in these three arrays.
        if(digi_01[k]!=arr_m0[i])
        {
            k=k+1;
            digi_01[k]=arr_m0[i];
            txt.printf("digi_01\r\n");
        }
        if(digi_11[l]!=arr_m1[i])
        {
            l=l+1;
            digi_11[l]=arr_m1[i];
            txt.printf("digi_11\r\n");
        }
        if(digi_21[m]!=arr_m2[i])
        {
            m=m+1;
            digi_21[m]=arr_m2[i];
            txt.printf("digi_21\r\n");
        }
        if(arr_m1[i]==-1)
        {
            count1++;//total number of minus one's
        }
        
        txt.printf("entered for loop %f %d %d\r\n",arr_m0[i],num0[1],num0[0]);
        
    }
    i=num0[0];
    while(i<=num0[1])
    {
        i++;
        if(arr_m1[i]!=2)
        {
            count2=i;//index of 2
        }
        if(arr_m1[i]==2)
        {
            break;
        }
   }
    //the digi arrays will now have 1 and -1 at the beginning and at the end definitely.
    //writing different cases now for identifying the digit
    //for identifying zero the following should hold true 212,2-12,212
    meand0=(num0[0]+num1[1])/2;
    txt.printf("digit0\r\n");
    txt.printf("the array values are %d %d %d %d %d %d %d %d %d %d 0\r\n",digi_01[0],digi_01[1],digi_01[2],digi_01[3],digi_01[4],digi_01[5],digi_01[6],digi_01[7],digi_01[8],digi_01[9]);
    txt.printf("the array values are %d %d %d %d %d %d %d %d %d %d 0\r\n",digi_11[0],digi_11[1],digi_11[2],digi_11[3],digi_11[4],digi_11[5],digi_11[6],digi_11[7],digi_11[8],digi_11[9]);
    txt.printf("the array values are %d %d %d %d %d %d %d %d %d %d 0\r\n",digi_21[0],digi_21[1],digi_21[2],digi_21[3],digi_21[4],digi_21[5],digi_21[6],digi_21[7],digi_21[8],digi_21[9]);
    if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==2)&&(digi_01[5]==1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==-1)&&(digi_11[3]==1)&&(digi_11[4]==-1)&&(digi_21[0]==-1)&&(digi_21[1]==1)&&(digi_21[2]==-1))
    {
        digit0=0;
        txt.printf("entered 0\r\n");
    }//ideal case
    
   
    //identifying one 1,1,1
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==-1)&&(digi_21[0]==-1)&&(digi_21[1]==1)&&(digi_21[2]==-1))
    {
        digit0=1;
    }//ideal case
    
   else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==-1))
    {
        if(arr_m0[meand0]==-1)
        {
            digit0=7;
        }
        else if(meand0<count2)
        {
            digit0=2;
        }
        else if(meand0>count2)
        {
            digit0=5;
        }
        
    }//ideal case
    //identifying three 12,12,12
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==2)&&(digi_11[3]==1)&&(digi_11[4]==-1))
    {
        if(meand0<count2)
        {
            digit0=3;
        }
        else if(meand0>count2)
        {
            digit0=6;
        }
    }//ideal case
    //identifying four 1-11,212,1
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==-1)&&(digi_01[3]==1)&&(digi_01[4]==-1)&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==2)&&(digi_11[3]==1)&&(digi_11[4]==-1))
    {
        digit0=4;
    }//ideal case
    //identifying five 21,212,12
    
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==2)&&(digi_01[5]==1)&&(digi_01[6]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==2)&&(digi_11[3]==1)&&(digi_11[4]==2)&&(digi_11[5]==1)&&(digi_11[6]==-1))
    {
        digit0=8;
    }//ideal case
    //identifying nine 212,212,12
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==2)&&(digi_01[5]==1)&&(digi_01[6]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==2)&&(digi_11[3]==1)&&(digi_11[4]==-1))
    {
        digit0=9;
    }//ideal case
    else
        digit0=2;
    
    
    
    ////////////////////////////////////////////identifying second digit////////////////////////////////////////////////////////
    
    txt.printf("digit0 is %d\r\n",digit0);
    txt.printf("digit1\r\n");
    k=0;
    for(k=0;k<20;k++)
    {
        digi_01[k]=0;
    }
    k=0;
    for(k=0;k<20;k++)
    {
        digi_11[k]=0;
    }
    k=0;
    for(k=0;k<20;k++)
    {
        digi_21[k]=0;
    }
    
    //identifying what values are present in the first second and third signals till num0[1]
    digi_01[0]=arr_m0[num0[1]];
    
    digi_11[0]=arr_m1[num0[1]];
    digi_21[0]=arr_m2[num0[1]];
    txt.printf("finished assigning digi_01[0]\r\n");
    k=0;l=0;m=0;count1=0;count2=0;
    i=num0[1];
    txt.printf("initiating i with num0[1]\r\n");
    while(i<num0[2])
    {
        txt.printf("entered while loop of digit1\r\n");
        i++;
       // storing the sequence of peaks of the first digit in these three arrays.
        if(digi_01[k]!=arr_m0[i])
        {
            k=k+1;
            digi_01[k]=arr_m0[i];
            txt.printf("a\r\n");
        }
        if(digi_11[l]!=arr_m1[i])
        {
            l=l+1;
            digi_11[l]=arr_m1[i];
            txt.printf("b\r\n");
        }
        if(digi_21[m]!=arr_m2[i])
        {
            m=m+1;
            digi_21[m]=arr_m2[i];
            txt.printf("c\r\n");
        }
        if(arr_m1[i]==-1)
        {
            count1++;
        }
        
    }
    count2=0;
    i=num0[1];
    while(i<=num0[2])
    {
        i++;
        if(arr_m1[i]!=2)
        {
            count2=i;
        }
         if(arr_m1[i]==2)
        {
            break;
        }
   }
   meand1=(num0[1]+num1[2])/2;
    //the digi arrays will now have 1 and -1 at the beginning and at the end definitely.
    //writing different cases now for identifying the digit
    //for identifying zero the following should hold true 212,2-12,212
    txt.printf("the array values are %d %d %d %d %d %d %d %d %d %d 0\r\n",digi_01[0],digi_01[1],digi_01[2],digi_01[3],digi_01[4],digi_01[5],digi_01[6],digi_01[7],digi_01[8],digi_01[9]);
    txt.printf("the array values are %d %d %d %d %d %d %d %d %d %d 0\r\n",digi_11[0],digi_11[1],digi_11[2],digi_11[3],digi_11[4],digi_11[5],digi_11[6],digi_11[7],digi_11[8],digi_11[9]);
    txt.printf("the array values are %d %d %d %d %d %d %d %d %d %d 0\r\n",digi_21[0],digi_21[1],digi_21[2],digi_21[3],digi_21[4],digi_21[5],digi_21[6],digi_21[7],digi_21[8],digi_21[9]);
    if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==2)&&(digi_01[5]==1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==-1)&&(digi_11[3]==1)&&(digi_11[4]==-1)&&(digi_21[0]==-1)&&(digi_21[1]==1)&&(digi_21[2]==-1))
    {
        digit1=0;
        txt.printf("entered 0\r\n");
    }//ideal case
    
    
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==-1)&&(digi_21[0]==-1)&&(digi_21[1]==1)&&(digi_21[2]==-1))
    {
        digit1=1;
    }//ideal case
    
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==-1))
    {
        if(arr_m1[meand1]==-1)
        {
            digit1=7;
        }
        else if(meand1<count2)
        {
            digit1=2;
        }
        else if(meand1>count2)
        {
            digit1=5;
        }
    }//ideal case
    //identifying three 12,12,12
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==2)&&(digi_11[3]==1)&&(digi_11[4]==-1))
    {
        if(meand1<count2)
        {
            digit1=3;
        }
        else if(meand1>count2)
        {
            digit1=6;
        }
    }//ideal case
    //identifying four 1-11,212,1
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==-1)&&(digi_01[3]==1)&&(digi_01[4]==-1)&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==2)&&(digi_11[3]==1)&&(digi_11[4]==-1))
    {
        digit1=4;
    }//ideal case
    //identifying five 21,212,12
   
   
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==-1))
    {
        digit1=7;
    }//ideal case
    //identifying 8 212,212,212
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==2)&&(digi_01[5]==1)&&(digi_01[6]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==2)&&(digi_11[3]==1)&&(digi_11[4]==2)&&(digi_11[5]==1)&&(digi_11[6]==-1))
    {
        digit1=8;
    }//ideal case
    //identifying nine 212,212,12
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==2)&&(digi_01[5]==1)&&(digi_01[6]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==2)&&(digi_11[3]==1)&&(digi_11[4]==-1))
    {
        digit1=9;
    }//ideal case
    txt.printf("Done identifying the second digit\r\n");
    txt.printf("second digit is %d\r\n",digit1);
    
    
    
    
    
    
    ///////////////////////////////////////////////////////////identifying third digit//////////////////////////////////////////////////////////////
    
    
    
    
    
    
    
    
    
    
    
    txt.printf("digit2\r\n");
    //identifying what values are present in the first second and third signals till num0[1]
    k=0;
    for(k=0;k<20;k++)
    {
        digi_01[k]=0;
    }
    k=0;
    for(k=0;k<20;k++)
    {
        digi_11[k]=0;
    }
    k=0;
    for(k=0;k<20;k++)
    {
        digi_21[k]=0;
    }
    
    
    digi_01[0]=arr_m0[num0[2]];
    digi_11[0]=arr_m1[num0[2]];
    digi_21[0]=arr_m2[num0[2]];
    k=0;l=0;m=0;count1=0;count2=0;
    i=num0[2];
    txt.printf("done initializing i for digit2\r\n");
    while(i<num0[3])
    {
        txt.printf("enteres while loop for identifying third digit\r\n");
        i++;
       // storing the sequence of peaks of the first digit in these three arrays.
        if(digi_01[k]!=arr_m0[i])
        {
            txt.printf("a\r\n");
            k=k+1;
            digi_01[k]=arr_m0[i];
        }
        if(digi_11[l]!=arr_m1[i])
        {
            txt.printf("b\r\n");
            l=l+1;
            digi_11[l]=arr_m1[i];
        }
        if(digi_21[m]!=arr_m2[i])
        {
            txt.printf("c\r\n");
            m=m+1;
            digi_21[m]=arr_m2[i];
        }
        if(arr_m1[i]==-1)
        {
            count1++;
        }
        
    }
    count2=0;
    i=num0[2];
    while(i<=num0[3])
    {
        i++;
        if(arr_m1[i]!=2)
        {
            count2=i;
        }
        if(arr_m1[i]==2)
        {
            break;
        }
   }
    //the digi arrays will now have 1 and -1 at the beginning and at the end definitely.
    //writing different cases now for identifying the digit
    //for identifying zero the following should hold true 212,2-12,212
    meand2=(num0[2]+num1[3])/2;
    txt.printf("the array values are %d %d %d %d %d %d %d %d %d %d 0\r\n",digi_01[0],digi_01[1],digi_01[2],digi_01[3],digi_01[4],digi_01[5],digi_01[6],digi_01[7],digi_01[8],digi_01[9]);
    txt.printf("the array values are %d %d %d %d %d %d %d %d %d %d 0\r\n",digi_11[0],digi_11[1],digi_11[2],digi_11[3],digi_11[4],digi_11[5],digi_11[6],digi_11[7],digi_11[8],digi_11[9]);
    txt.printf("the array values are %d %d %d %d %d %d %d %d %d %d 0\r\n",digi_21[0],digi_21[1],digi_21[2],digi_21[3],digi_21[4],digi_21[5],digi_21[6],digi_21[7],digi_21[8],digi_21[9]);
    if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==2)&&(digi_01[5]==1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==-1)&&(digi_11[3]==1)&&(digi_11[4]==-1)&&(digi_21[0]==-1)&&(digi_21[1]==1)&&(digi_21[2]==-1))
    {
        digit2=0;
        txt.printf("entered 0\r\n");
    }//ideal case
    
    
    //identifying one 1,1,1
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==-1)&&(digi_21[0]==-1)&&(digi_21[1]==1)&&(digi_21[2]==-1))
    {
        digit2=1;
    }//ideal case
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==-1))
    {
        if(arr_m2[meand2]==-1)
        {
            digit1=7;
        }
        else if(meand0<count2)
        {
            digit2=2;
        }
        else if(meand0>count2)
        {
            digit2=5;
        }
    }//when there are three peaks
    //identifying two 12,212,21
    
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==2)&&(digi_11[3]==1)&&(digi_11[4]==-1))
    {
        if(meand2<count2)
        {
            digit2=3;
        }
        else if(meand2>count2)
        {
            digit2=6;
        }
    }//ideal case
    //identifying four 1-11,212,1
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==-1)&&(digi_01[3]==1)&&(digi_01[4]==-1)&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==2)&&(digi_11[3]==1)&&(digi_11[4]==-1))
    {
        digit2=4;
    }//ideal case
    //identifying five 21,212,12
    
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==-1))
    {
        digit2=7;
    }//ideal case
    //identifying 8 212,212,212
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==2)&&(digi_01[5]==1)&&(digi_01[6]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==2)&&(digi_11[3]==1)&&(digi_11[4]==2)&&(digi_11[5]==1)&&(digi_11[6]==-1))
    {
        digit2=8;
    }//ideal case
    //identifying nine 212,212,12
    else if((digi_01[0]==-1)&&(digi_01[1]==1)&&(digi_01[2]==2)&&(digi_01[3]==1)&&(digi_01[4]==2)&&(digi_01[5]==1)&&(digi_01[6]==-1)&&(digi_11[0]==-1)&&(digi_11[1]==1)&&(digi_11[2]==2)&&(digi_11[3]==1)&&(digi_11[4]==-1))
    {
        digit2=9;
    }//ideal case
    
    //finished identifying all the three digits hurray!!
    
    
    txt.printf("digit0 is %d\r\n",digit0);
    txt.printf("digit1 is %d\r\n",digit1);
    txt.printf("digit2 is %d\r\n",digit2);
    
    
    
    
           
    
    
    
   
}
