#include "mbed.h"

DigitalOut myled(LED1);
DigitalOut myled1(LED2);
DigitalOut myled2(LED3);
DigitalOut myled3(LED4);

AnalogIn ldr1(p15);
AnalogIn ldr2(p16);
AnalogIn ldr3(p17);

Serial txt(USBTX, USBRX); // tx, rx
//txt.baud(9600);

LocalFileSystem local("local");
float sample[3],sample_p[3];
int sample_int[3],arr0[400],arr1[400],arr2[400],count=0,N=0,arr_d0[100][3],arr_d1[100][3],arr_d2[100][3];
char control='c';
int flag=0,flag_s=0;

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
    
        myled = 1;
    
        sample[0] = ldr1.read();
        sample[1] = ldr2.read();
        sample[2] = ldr3.read();
        
        sample_int[0] = 100*sample[0];        
        sample_int[1] = 100*sample[1];
        sample_int[2] = 100*sample[2];
        
        //sample[0] = sample[0] + 0.02;
        
        if(sample_int[0]<=50)
        {
            sample_int[0] = 0;
        }
        
        
        if(sample_int[1]<=50)
        {
            sample_int[1] = 0;
        }
 
       ////////////
        
        if(sample_int[2]<=50)
        {
            sample_int[2] = 0;
        }  
        
        if(((sample_int[0]==0)&&(sample_int[1]==0))&&(sample_int[2]==0))
        {
            if(flag==0)
            {
                fprintf(fp," %d, %d, %d \r\n", sample_int[0],sample_int[1],sample_int[2]);
                
                arr0[count]=sample_int[0];
                arr1[count]=sample_int[1];
                arr2[count]=sample_int[2];
                
                count++;
            }
            
            flag = 1;
        }
       
        else
        {
            flag=0;
            //flag_s=0;
            fprintf(fp," %d, %d, %d \r\n", sample_int[0],sample_int[1],sample_int[2]);
            
            arr0[count]=sample_int[0];
            arr1[count]=sample_int[1];
            arr2[count]=sample_int[2];
            
            count++;
        }
                
        sample_p[0] = sample_int[0];
        sample_p[1] = sample_int[1];
        sample_p[2] = sample_int[2];
        
        wait_ms(24);   
        
        myled =0;
        
        wait_ms(24);
        
        if(control == 'b')
        {
            break;
        }
    }
    
    fclose(fp);
    
    txt.printf("detecting digits ... \r\n");
    
    N=count;
    
    int digi_flag=0,digi_index[3];
    int temp_i=2,i;
    
    for(i=N-1;i>=0;i--)
    {
        if(((arr0[i]==0)&&(arr1[i]==0))&&(arr2[i]==0))
        {
            digi_index[temp_i] = i;
            digi_flag++;
            temp_i--;
            
            if(temp_i==-1)
            {
                break;
            }
         }
        
     }
     
     int digi_len[3];
     
     digi_len[2] = N - digi_index[2];
     digi_len[1] = digi_index[2] - digi_index[1] - 1;
     digi_len[0] = digi_index[1] - digi_index[0] - 1;
     
     int no_cor0=0,no_cor1=0,no_cor2=0, Ts[3] = {0,0,0}, cor[2][2][3],cntc0=0,cntc1=0,cntc2=0 ;
     int no_cort0=0,no_cort1=0,no_cort2=0,no_corb0=0,no_corb1=0,no_corb2=0,Tsc=0;
     //int cor0_pos=0,
     
     
     for(i=digi_index[0];i<digi_index[1];i++)
     {
        if(arr0[i]>=66)
        {
            cntc0++;
        }
        if(arr1[i]>=68)
        {
            cntc1++;
        }
        if(arr2[i]>=66)
        {
            cntc2++;
        }
     }
     
     
        
        if(cntc0<=3)
        {
            no_cort0=0;
        }
        else if(cntc0<=(digi_len[0]/2))
        {
            no_cort0=1;
        }
        else
        {
            no_cort0=2;
        }
        
        if(cntc2<=3)
        {
            no_corb0=0;
        }
        else if(cntc2<=(digi_len[0]/2))
        {
            no_corb0=1;
        }
        else
        {
            no_corb0=2;
        }
        
        no_cor0 = no_cort0 + no_corb0;
        
        if(cntc1<=3)
        {
            Ts[0]=0;
        }
        else if(cntc1<=(digi_len[0]/2))
        {
            Ts[0]=1;
        }
        else
        {
            Ts[0]=2;
        }
        
        cntc0=0;cntc1=0;cntc2=0;
        
        
        for(i=digi_index[1];i<digi_index[2];i++)
     {
        if(arr0[i]>=66)
        {
            cntc0++;
        }
        if(arr1[i]>=68)
        {
            cntc1++;
        }
        if(arr2[i]>=66)
        {
            cntc2++;
        }
     }
     
     
        
        if(cntc0<=3)
        {
            no_cort1=0;
        }
        else if(cntc0<=(digi_len[1]/2))
        {
            no_cort1=1;
        }
        else
        {
            no_cort1=2;
        }
        
        if(cntc2<=3)
        {
            no_corb1=0;
        }
        else if(cntc2<=(digi_len[1]/2))
        {
            no_corb1=1;
        }
        else
        {
            no_corb1=2;
        }
        
        no_cor1 = no_cort1 + no_corb1;
        
        if(cntc1<=3)
        {
            Ts[1]=0;
        }
        else if(cntc1<=(digi_len[1]/2))
        {
            Ts[1]=1;
        }
        else
        {
            Ts[1]=2;
        }
        
        
        cntc0=0;cntc1=0;cntc2=0;
        
        for(i=digi_index[2];i<N;i++)
        {
            if(arr0[i]>=69)
            {
                cntc0++;
            }
            if(arr1[i]>=68)
            {
                cntc1++;
            }
            if(arr2[i]>=69)
            {
                cntc2++;
            }
        }
     
     
        
        if(cntc0<=3)
        {
            no_cort2=0;
        }
        else if(cntc0<=(digi_len[2]/2))
        {
            no_cort2=1;
        }
        else
        {
            no_cort2=2;
        }
        
        if(cntc2<=3)
        {
            no_corb2=0;
        }
        else if(cntc2<=(digi_len[2]/2))
        {
            no_corb2=1;
        }
        else
        {
            no_corb2=2;
        }
        
        no_cor2 = no_cort2 + no_corb2;
        
        if(cntc1<=3)
        {
            Ts[2]=0;
        }
        else if(cntc1<=(digi_len[2]/2))
        {
            Ts[2]=1;
        }
        else
        {
            Ts[2]=2;
        }
        
        
         
    txt.printf("Closing File...\n");
    txt.printf("digits : %d, at %d, %d, %d\r\n",digi_flag,digi_index[0],digi_index[1],digi_index[2]);
    txt.printf("corners & Ts : \r\n 1st digit %d,%d,%d \r\n 2nd digit %d,%d,%d \r\n 3rd digit %d,%d,%d \r\n",no_cort0,Ts[0],no_corb0,no_cort1,Ts[1],no_corb1,no_cort2,Ts[2],no_corb2); 
    
   
}
