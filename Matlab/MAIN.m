Arrv_rate = [50, 100, 200, 300];
global trans_rate frame_size ack_size rts_size cts_size sim_time sifs_time difs_time slot_time;
global sim_slot  sifs_slot  difs_slot cts_slot rts_slot ack_slot frame_slot;


    trans_rate = 6000000; 
    frame_size = 1500*8; 
    ack_size = 30*8; 
    rts_size = 30*8; 
    cts_size = 30*8; 
    sim_time = 10; 
    sifs_time = 0.000010;
    difs_time = 0.000040;
    slot_time = 0.000020; 
    
    sim_slot = ceil(sim_time/slot_time); 
    frame_slot = ceil(frame_size/trans_rate/slot_time); 
    ack_slot = ceil(ack_size/trans_rate/slot_time); 
    rts_slot = ceil(rts_size/trans_rate/slot_time); 
    cts_slot = ceil(cts_size/trans_rate/slot_time); 
    sifs_slot = ceil(sifs_time/slot_time); 
    difs_slot = ceil(difs_time/slot_time); 



Throughput_A_CON=zeros(1,4);
Throughput_A_HID=zeros(1,4);
Throughput_A_CON_RTSCTS=zeros(1,4);
Throughput_A_HID_RTSCTS=zeros(1,4);
Throughput_B_CON=zeros(1,4);
Throughput_B_HID=zeros(1,4);
Throughput_B_CON_RTSCTS=zeros(1,4);
Throughput_B_HID_RTSCTS=zeros(1,4);
Collision_A_CON=zeros(1,4);
Collision_A_HID=zeros(1,4);
Collision_A_CON_RTSCTS=zeros(1,4);
Collision_A_HID_RTSCTS=zeros(1,4);
Collision_B_CON=zeros(1,4);
Collision_B_HID=zeros(1,4);
Collision_B_CON_RTSCTS=zeros(1,4);
Collision_B_HID_RTSCTS=zeros(1,4);

FI_CON_AB=zeros(1,4);
FI_HID_AB=zeros(1,4);
FI_CON_AB_RTSCTS=zeros(1,4);
FI_HID_AB_RTSCTS=zeros(1,4);

x=cat(2,Arrv_rate,Arrv_rate);

for i=1:4
    
output_CON_DCF_1=CON_CSMA_DCF(Arrv_rate(i),Arrv_rate(i));
%output_HID_DCF_1=HID_CSMA_DCF(Arrv_rate(i),Arrv_rate(i));
%output_CON_DCF_RTSCTS_1=CON_CSMA_DCF_RTSCTS(Arrv_rate(i),Arrv_rate(i));
%output_HID_DCF_RTSCTS_1=HID_CSMA_DCF_RTSCTS(Arrv_rate(i),Arrv_rate(i));

Throughput_A_CON(i)=output_CON_DCF_1(1);
%Throughput_A_HID(i)=output_HID_DCF_1(1);
%Throughput_A_CON_RTSCTS(i)=output_CON_DCF_RTSCTS_1(1);
%Throughput_A_HID_RTSCTS(i)=output_HID_DCF_RTSCTS_1(1);


Throughput_B_CON(i)=output_CON_DCF_1(2);
%Throughput_B_HID(i)=output_HID_DCF_1(2);
%Throughput_B_CON_RTSCTS(i)=output_CON_DCF_RTSCTS_1(2);
%Throughput_B_HID_RTSCTS(i)=output_HID_DCF_RTSCTS_1(2);


%Collision_A_CON(i)=output_CON_DCF_1(3);
%Collision_A_HID(i)=output_HID_DCF_1(3);
%Collision_A_CON_RTSCTS(i)=output_CON_DCF_RTSCTS_1(3);
%Collision_A_HID_RTSCTS(i)=output_HID_DCF_RTSCTS_1(3);

%Collision_B_HID(i)=output_HID_DCF_1(4);
%Collision_B_CON(i)=output_CON_DCF_1(4);
%Collision_B_HID_RTSCTS(i)=output_HID_DCF_RTSCTS_1(4);
%Collision_B_CON_RTSCTS(i)=output_CON_DCF_RTSCTS_1(4);


%FI_CON_AB(i)=output_CON_DCF_1(5);
%FI_HID_AB(i)=output_HID_DCF_1(5);
%FI_CON_AB_RTSCTS(i)=output_CON_DCF_RTSCTS_1(5);
%FI_HID_AB_RTSCTS(i)=output_HID_DCF_RTSCTS_1(5);

end

figure(1)

plot(Arrv_rate,Throughput_A_CON,'-bo', Arrv_rate,Throughput_A_HID,'c*',Arrv_rate,Throughput_A_CON_RTSCTS,'--', Arrv_rate,Throughput_A_HID_RTSCTS,':');
title('Node A \rightarrow C');
legend({'Concurrent CSMA','Hidden CSMA','Concurrent CSMA CA','Hidden CSMA CA'},'Location','northwest','NumColumns',2);%
ylabel('T (Kbps)');
xlabel('\lambda (frames/sec)');
axis([0,400,0,4000]);


% figure(2)
% title('Node B Throughput T (Kbps) vs. rate λ (frames/sec) for scenarios A and B, and CSMA implementations 1 and 2');
% plot(Arrv_rate,Throughput_B_CON,'-bo', Arrv_rate,Throughput_B_HID,'c*',Arrv_rate,Throughput_B_CON_RTSCTS,'--', Arrv_rate,Throughput_B_HID_RTSCTS,':');
% title('Node A: λA=λB');
% legend({'Concurrent CSMA','Hidden CSMA','ConcurrentCSMA VA','Hidden CSMA VA'},'Location','northwest','NumColumns',2);%
% ylabel('$T$ (Kbps)');
% xlabel('$λ$ (frames/sec)');
% axis([0,400,0,4000]);
% 
% 
% figure(3)
% plot(Arrv_rate,Collision_A_CON,'-bo', Arrv_rate,Collision_A_HID,'c*',Arrv_rate,Collision_A_CON_RTSCTS,'--', Arrv_rate,Collision_A_HID_RTSCTS,':');
% title('Node A: Number of collision λA=λB ');
% legend({'Concurrent CSMA','Hidden CSMA','ConcurrentCSMA VA','Hidden CSMA VA'},'Location','northwest','NumColumns',2);%
% ylabel('No of collision');
% xlabel('$λ$ (frames/sec)');
% axis([0,400,0,5000]);
% 
% figure(4)
% 
% plot(Arrv_rate,Collision_B_CON,'-bo', Arrv_rate,Collision_B_HID,'c*',Arrv_rate,Collision_B_CON_RTSCTS,'--', Arrv_rate,Collision_B_HID_RTSCTS,':');
% title('Node B: Number of collision λA=λB ');
% legend({'Concurrent CSMA','Hidden CSMA','ConcurrentCSMA VA','Hidden CSMA VA'},'Location','northwest','NumColumns',2);%
% ylabel('No of collision');
% xlabel('$λ$ (frames/sec)');
% axis([0,400,0,5000]);
% 
% figure(5)
% 
% plot(Arrv_rate,FI_CON_AB,'-bo', Arrv_rate,FI_HID_AB,'c*',Arrv_rate,FI_CON_AB_RTSCTS,'--', Arrv_rate,FI_HID_AB_RTSCTS,':');
% title('Fairness Index F I λA=λB');
% legend({'Concurrent CSMA','Hidden CSMA','ConcurrentCSMA VA','Hidden CSMA VA'},'Location','northwest','NumColumns',2);%
% ylabel('FI Index');
% xlabel('$λ$ (frames/sec)');
% axis([0,400,0,5]);

%%
for i=1:4
    
output_CON_DCF_1=CON_CSMA_DCF(2*Arrv_rate(i),Arrv_rate(i));
output_HID_DCF_1=HID_CSMA_DCF(2*Arrv_rate(i),Arrv_rate(i));
output_CON_DCF_RTSCTS_1=CON_CSMA_DCF_RTSCTS(2*Arrv_rate(i),Arrv_rate(i));
output_HID_DCF_RTSCTS_1=HID_CSMA_DCF_RTSCTS(2*Arrv_rate(i),Arrv_rate(i));

Throughput_A_CON(i)=output_CON_DCF_1(1);
Throughput_A_HID(i)=output_HID_DCF_1(1);
Throughput_A_CON_RTSCTS(i)=output_CON_DCF_RTSCTS_1(1);
Throughput_A_HID_RTSCTS(i)=output_HID_DCF_RTSCTS_1(1);


Throughput_B_CON(i)=output_CON_DCF_1(2);
Throughput_B_HID(i)=output_HID_DCF_1(2);
Throughput_B_CON_RTSCTS(i)=output_CON_DCF_RTSCTS_1(2);
Throughput_B_HID_RTSCTS(i)=output_HID_DCF_RTSCTS_1(2);


Collision_A_CON(i)=output_CON_DCF_1(3);
Collision_A_HID(i)=output_HID_DCF_1(3);
Collision_A_CON_RTSCTS(i)=output_CON_DCF_RTSCTS_1(3);
Collision_A_HID_RTSCTS(i)=output_HID_DCF_RTSCTS_1(3);

Collision_B_HID(i)=output_HID_DCF_1(4);
Collision_B_CON(i)=output_CON_DCF_1(4);
Collision_B_HID_RTSCTS(i)=output_HID_DCF_RTSCTS_1(4);
Collision_B_CON_RTSCTS(i)=output_CON_DCF_RTSCTS_1(4);


FI_CON_AB(i)=output_CON_DCF_1(5);
FI_HID_AB(i)=output_HID_DCF_1(5);
FI_CON_AB_RTSCTS(i)=output_CON_DCF_RTSCTS_1(5);
FI_HID_AB_RTSCTS(i)=output_HID_DCF_RTSCTS_1(5);

end

figure(6)

plot(Arrv_rate,Throughput_A_CON,'-bo', Arrv_rate,Throughput_A_HID,'c*',Arrv_rate,Throughput_A_CON_RTSCTS,'--', Arrv_rate,Throughput_A_HID_RTSCTS,':');
title('Node A: λA=2λB');
legend({'Concurrent CSMA','Hidden CSMA','ConcurrentCSMA VA','Hidden CSMA VA'},'Location','northwest','NumColumns',2);%
ylabel('$T$ (Kbps)');
xlabel('$λ$ (frames/sec)');
axis([0,400,0,8000]);


figure(7)

plot(Arrv_rate,Throughput_B_CON,'-bo', Arrv_rate,Throughput_B_HID,'c*',Arrv_rate,Throughput_B_CON_RTSCTS,'--', Arrv_rate,Throughput_B_HID_RTSCTS,':');
title('Node B: λA=2λB');
legend({'Concurrent CSMA','Hidden CSMA','ConcurrentCSMA VA','Hidden CSMA VA'},'Location','northwest','NumColumns',2);%
ylabel('$T$ (Kbps)');
xlabel('$λ$ (frames/sec)');
axis([0,400,0,4000]);


figure(8)
plot(Arrv_rate,Collision_A_CON,'-bo', Arrv_rate,Collision_A_HID,'c*',Arrv_rate,Collision_A_CON_RTSCTS,'--', Arrv_rate,Collision_A_HID_RTSCTS,':');
title('Node A: Number of collision λA=2λB ');
legend({'Concurrent CSMA','Hidden CSMA','ConcurrentCSMA VA','Hidden CSMA VA'},'Location','northwest','NumColumns',2);%
ylabel('No of collision');
xlabel('$λ$ (frames/sec)');
axis([0,400,0,5000]);

figure(9)

plot(Arrv_rate,Collision_B_CON,'-bo', Arrv_rate,Collision_B_HID,'c*',Arrv_rate,Collision_B_CON_RTSCTS,'--', Arrv_rate,Collision_B_HID_RTSCTS,':');
title('Node B: Number of collision λA=2λB ');
legend({'Concurrent CSMA','Hidden CSMA','ConcurrentCSMA VA','Hidden CSMA VA'},'Location','northwest','NumColumns',2);%
ylabel('No of collision');
xlabel('$λ$ (frames/sec)');
axis([0,400,0,5000]);

figure(10)
title('Fairness Index F I when λ A = 2λ C (four lines in total)');
plot(Arrv_rate,FI_CON_AB,'-bo', Arrv_rate,FI_HID_AB,'c*',Arrv_rate,FI_CON_AB_RTSCTS,'--', Arrv_rate,FI_HID_AB_RTSCTS,':');
legend({'Concurrent CSMA','Hidden CSMA','ConcurrentCSMA VA','Hidden CSMA VA'},'Location','northwest','NumColumns',2);%
ylabel('FI λ A = 2λ C ');
xlabel('$λ$ (frames/sec)');
axis([0,400,0,8]);



