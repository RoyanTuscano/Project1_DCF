function output_data=CON_CSMA_DCF(Arrv_rate_A,Arrv_rate_C)

global sim_slot  sifs_slot  difs_slot ack_slot frame_slot;
cw0 = 4;                                % Initial backoff time range = [ 0 - cw0 ]
temp_cw1 = cw0;                         % This is a temporary variable used for calculating the random backoff time range for Node A
temp_cw2 = cw0;                         % This is a temporary variable used for calculating the random backoff time range for Node B
cw1 = 0;                                % This is a variable used for calculating the random backoff time range for Node A
cw2 = 0;                                % This is a variable used for calculating the random backoff time range for Node B
no_ACK = 0;                             % This variable is used to track that no ACK is sent when a collision occurs
Node_status = zeros(4,sim_slot);        % Status for each nodes, Node_status[1-4,:] = Channel 1 - Channel 4. 0 channel is iddle, 1 channel is busy
NAV = zeros(4,sim_slot);                % Network Allocation Vector NAV[1-4,:] = Channel 1 - Channel 4. NAV value for each time slot
channel = zeros(1,sim_slot);            % Status for the carrier 
DIFS_slots = zeros(2,sim_slot);         % Distributed InterFrame Space DIFS[1-2,:] = Channel 1 - Channel 2. DIFS value for each time slot
bck_of = [ 0 0];                        % backoff status of each stations
num_of_collision = [0 0];               % number of collisions for each TX Stations
send_frame = [0 0];                     % send frame status for each TX stations
back_off_finish = [ 0 0];               % backoff finish status for each stations
send_either_frame = 0;                  % send either frame status variable
Traffic_A = TRAFFIC_GEN(Arrv_rate_A);   % Traffic values generated according to poisson arrival rates channel A
Traffic_C = TRAFFIC_GEN(Arrv_rate_C);   % Traffic values generated according to poisson arrival rates channel C
a = size(Traffic_A);                    % size of Traffic times for channel A
c = size(Traffic_C);                    % size of Traffic times for channel C
Traffic_A_Received=zeros(a);            % Status for channel A TX received
Traffic_C_Received=zeros(c);            % Status for channel C TX received
num_success_1 = 0;                      % Number of success for TX of channel A
num_success_2 = 0;                      % Number of success for TX of channel C
collision_flag_1=0;                     % Collision counter for channel A
collision_flag_2=0;                     % Collision counter for channel C
i=1;
while i<sim_slot
    i=i+1;
    if channel(i) == 0                  % Channel is iddle
        for s = 1:a(2)
            if Traffic_A(s) ~= 0 && Traffic_A_Received(s) == 0 && i>=Traffic_A(s) % Channel A & B are free
                Node_status(1,i) = 1;
                s1 = s;
                break;
            end
        end
        for s = 1:c(2)
            if Traffic_C(s) ~= 0 && Traffic_C_Received(s) == 0 && i>=Traffic_C(s) % Channel C & D are free
                Node_status(2,i) = 1;
                s2 = s;
                break;
            end
        end
    end
    for node = 1:2
        if i >= 500000
            break;
        elseif(Node_status(node,i) == 1 && channel(i) == 0 && DIFS_slots(node,i - 1) == 0 && DIFS_slots(node,i) == 0 && bck_of(node) == 0)
            % if carrier is free & channel A/B is ready to TX, the DIFS is done & backoff is done 
            DIFS_slots(node, i+1:i+difs_slot) = 1;
            Node_status(node, i+1:i+difs_slot+1) = 1;
        elseif(Node_status(node,i) == 1 && channel(i) == 0 && DIFS_slots(node,i - 1) == 1  && DIFS_slots(node,i) == 0 && bck_of(node)==0)
            % if carrier is free & channel A/B is ready to TX, DIFF is ON & backoff is done
            if node == 1
                if collision_flag_1 == 0    % if no previous collisions
                    cw1 = randi([0,cw0-1],1,1);
                else
                    if collision_flag_1 < 10 % if previous collisions
                        %fprintf("collision_flag_1 %i\n",collision_flag_1);
                        temp_cw1 = (2^collision_flag_1)*cw0;
                        if(temp_cw1<1024)
                            cw1 = randi([0,temp_cw1-1],1,1);
                        else
                            cw1 = randi([0,1024],1,1);      % Why don't we just max the CW?
                        end
                    else
                        %fprintf("Rut 1,");
                        collision_flag_1=0;
                        Traffic_A_Received(s1) = 1;
                        send_ACK = 0;
                        send_either_frame = 0;
                        send_frame(1)  = 0;
                    end
                end
                Node_status(node,i+1:i+cw1) = 1;
                bck_of(node) = 1;
            elseif node == 2
                if collision_flag_2 == 0
                    cw2 = randi([0,cw0-1],1,1);
                    
                else
                    if collision_flag_2 < 10
                        temp_cw2 = (2^collision_flag_2)*cw0;
                        %fprintf("collision_flag_2 %i\n",collision_flag_2);
                        if(temp_cw2<1024)
                            cw2 = randi([0,temp_cw2-1],1,1);
                        else
                            cw2 = randi([0,1024],1,1);
                        end
                    else
                        %fprintf("Rut 2,");
                        collision_flag_2=0;
                        Traffic_C_Received(s2) = 1;
                        send_ACK = 0;
                        send_either_frame = 0;
                        send_frame(2)  = 0;
                    end
                end
                Node_status(node,i+1:i+cw2) = 1;
                bck_of(node) = 1;
            end
        elseif(Node_status(node,i) == 1 && channel(i) == 0 && bck_of(node) == 1 &&  send_frame(node) == 0)
            % carrier is free, channel A/B is ready to send, backoff node
            % is not done, frame was not sent
            if node == 1
                if cw1 > 0
                    cw1 = cw1 - 1;
                    Node_status(1,i+1) = 1;
                end
                if cw1 == 0
                    send_frame(1) = 1;
                    bck_of(node) = 0;
                    back_off_finish(node) = 1;
                    Node_status(node,i+1:i+frame_slot) = 1;
                    send_either_frame = send_frame(1) || send_frame(2);
                end
            elseif node == 2
                if cw2 > 0
                    cw2 = cw2 - 1;
                    Node_status(2,i+1) = 1;
                end
                if cw2 == 0
                    send_frame(2) = 1;
                    bck_of(node) = 0;
                    back_off_finish(node) = 1;
                    Node_status(2,i+1:i+frame_slot) = 1;
                    send_either_frame = send_frame(1) || send_frame(2);
                end
            end
            if send_either_frame == 1   %  set Node status to mark that the station is sending
                if send_frame(1)==1 && send_frame(2)==0
                    Node_status(1,i:i+frame_slot+sifs_slot+ack_slot+1) = 1;
                elseif send_frame(1)==0 && send_frame(2)==1
                    Node_status(2,i:i+frame_slot+sifs_slot+ack_slot+1) = 1;
                elseif send_frame(1)==1 && send_frame(2)==1
                    Node_status(1,i:i+frame_slot+sifs_slot+ack_slot+1) = 1;
                    Node_status(2,i:i+frame_slot+sifs_slot+ack_slot+1) = 1;
                    no_ACK = 1;
                    
                end
            end
        elseif(Node_status(node,i) == 1 && bck_of(node) == 0 && back_off_finish(node) == 1 && send_frame(node) == 1)
            % channel A/B is ready to send, backoff is done, channel A/B
            % just finished backoff, frame is sent
            channel(i:i+frame_slot+sifs_slot+ack_slot) = 1;
            back_off_finish(node) = 0;
            NAV(node,i) = frame_slot+sifs_slot+ack_slot;
            NAV(node,i+1) = NAV(node,i) - 1;
            Node_status(node,i+1 : i+frame_slot) = 1;
            if send_frame(1) == 1 && send_frame(2) == 1
                no_ACK = 1;
                num_of_collision(node) = num_of_collision(node) + 1;
                if node==1
                    collision_flag_1=1+collision_flag_1;
                else
                    collision_flag_2=1+collision_flag_2;
                end
            end
            send_frame(node) = 0;
        elseif(Node_status(node,i) == 1 && channel(i) == 1 && bck_of(node) == 1) 
            % channel A/B is ready to TX, carrier is busy, backoff is still going
            if node == 1
                Node_status(node,i+1) = 1;
                if channel(i+1) == 0
                    send_either_frame = 0;
                end
            elseif node == 2
                Node_status(node,i+1) = 1;
                if channel(i+1) == 0
                    send_either_frame = 0;
                end
            end
        elseif channel(i) == 1 && NAV(node,i) > sifs_slot+ack_slot 
            % channel A/B is ready to TX, NAV is still going
            NAV(node,i+1) = NAV(node,i) - 1;
            if NAV(node,i+1) == sifs_slot+ack_slot
                if Node_status(1,i) == 1 && node == 1
                    Node_status(3,i:i+sifs_slot+ack_slot) = 1;
                    Node_status(1,i:i+1) = 0;
                elseif Node_status(2,i) == 1 && node == 2
                    Node_status(4,i:i+sifs_slot+ack_slot) = 1;
                    Node_status(2,i:i+1) = 0;
                end
            end
        elseif channel(i) == 1 && (Node_status(3,i) == 1 || Node_status(4,i) == 1) && NAV(node,i) > ack_slot
            % Channel B or D is getting ready to reply
            NAV(node,i+1) = NAV(node,i) - 1;
        elseif channel(i) == 1 && (Node_status(3,i) == 1 || Node_status(4,i) == 1) && NAV(node,i) == ack_slot
            % Channel B or D is ready to reply
            if no_ACK == 1
                NAV(node,i+1) = NAV(node,i) - 2;
                no_ACK = 0;
            else
                send_ACK = 1;
                NAV(node,i+1) = NAV(node,i) - 1;
            end
        elseif (channel(i) == 1 && (Node_status(3,i) == 1 || Node_status(4,i) == 1) && NAV(node,i) == 1)
            % Channel B or D is replying
            if Node_status(3,i) == 1
                if send_ACK == 1
                    Traffic_A_Received(s1) = 1;
                    num_success_1=num_success_1+1;
                    send_ACK = 0;
                    send_either_frame = 0;
                    send_frame(1)  = 0;
                    Node_status(1,i:sim_slot) = 0;
                    collision_flag_1=0;
                end
            elseif Node_status(4,i) == 1
                if send_ACK == 1
                    Traffic_C_Received(s2) = 1;
                    num_success_2=num_success_2+1;
                    send_ACK = 0;
                    send_either_frame = 0;
                    send_frame(2)  = 0;
                    Node_status(2,i:sim_slot) = 0;
                    collision_flag_2=0;
                end
            end
            NAV(node,i) = NAV(node,i) - 1;
        end
    end
end
Throughput1 = (num_success_1)*1500*8/10/1000;
Throughput2 = (num_success_2)*1500*8/10/1000;
FI = num_success_1/num_success_2;
fprintf("********CON***************\n");
fprintf('Frames in A (%d) = %d Frames in C (%d) = %d\n',Arrv_rate_A,a(2),Arrv_rate_C,c(2));
fprintf('Success in A (%d) = %d Success in C (%d) = %d\n',Arrv_rate_A,num_success_1,Arrv_rate_C,num_success_2);
fprintf('collisions in A (%d) = %d collisions in C (%d) = %d\n',Arrv_rate_A,num_of_collision(1),Arrv_rate_C,num_of_collision(2));
output_data=[Throughput1 Throughput2 num_of_collision(1) num_of_collision(2) FI];
end