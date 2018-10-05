function output_data=HID_CSMA_DCF_RTSCTS(Arrv_rate_A,Arrv_rate_C)
    global sim_slot  sifs_slot  difs_slot cts_slot rts_slot ack_slot frame_slot;
    sendCts = 0;
    cw0 = 4;
    temp_cw1 = cw0;
    temp_cw2 = cw0;
    cw1 = 0;
    cw2 = 0;
    num_success_1 = 0;
    num_success_2 = 0;
    no_CTS = [0 0];
    Node_status = zeros(4,sim_slot);
    NAV = zeros(2,sim_slot);
    channel_s = zeros(2,sim_slot);
    DIFS_slots = zeros(2,sim_slot);
    bck_of = [ 0 0];
    num_of_collision = [0 0];
    send_frame = [0 0];
    back_off_finish = [ 0 0];
    send_either_frame = 0;
    collision_flag_1=0;
    collision_flag_2=0;
    Traffic_A = TRAFFIC_GEN(Arrv_rate_A);
    Traffic_C = TRAFFIC_GEN(Arrv_rate_C);
    a = size(Traffic_A);
    c = size(Traffic_C);
    Traffic_A_Received=zeros(a);
    Traffic_C_Received=zeros(c);
    i=1;
    while i<sim_slot
        i=i+1;
        if channel_s(1,i) ==0
            for s = 1:a(2)
                if Traffic_A(s) ~= 0 && Traffic_A_Received(s) == 0 && i>=Traffic_A(s)
                    Node_status(1,i) = 1;
                    s1 = s;
                    break;
                end
            end
        end
        if channel_s(2,i) == 0
            for s = 1:c(2)
                if Traffic_C(s) ~= 0 && Traffic_C_Received(s) == 0 && i>=Traffic_C(s)
                    Node_status(2,i) = 1;
                    s2 = s;
                    break;
                end
            end
        end
        for node = 1:2
            if i >= 500000
                break;
            elseif(Node_status(node,i) == 1 && channel_s(node,i) == 0 && DIFS_slots(node,i - 1) == 0 && DIFS_slots(node,i) == 0 && bck_of(node) == 0)
                DIFS_slots(node, i+1:i+difs_slot) = 1;
                Node_status(node, i+1:i+difs_slot+1) = 1;
            elseif(Node_status(node,i) == 1 && channel_s(node,i) == 0 && DIFS_slots(node,i - 1) == 1  && DIFS_slots(node,i) == 0 && bck_of(node)==0)
                if node == 1
                    if collision_flag_1 == 0
                        cw1 = randi([0,cw0-1],1,1);
                    else
                        if collision_flag_1 < 10
                            % fprintf("collision_flag_1 %i\n",collision_flag_1);
                            temp_cw1 = (2^collision_flag_1)*cw0;
                            if(temp_cw1<1024)
                                cw1 = randi([0,temp_cw1-1],1,1);
                            else
                                cw1 = randi([0,1024-1],1,1);
                            end
                        else
                            % fprintf("Rut 1,");
                            collision_flag_1=0;
                            Traffic_A_Received(s1) = 1;
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
                            
                            if(temp_cw2<1024)
                                cw2 = randi([0,temp_cw2-1],1,1);
                            else
                                cw2 = randi([0,1024],1,1);
                            end
                        else
                            
                            collision_flag_2=0;
                            Traffic_C_Received(s2) = 1;
                            send_either_frame = 0;
                            send_frame(2)  = 0;
                        end
                    end
                    Node_status(node,i+1:i+cw2) = 1;
                    bck_of(node) = 1;
                end
            elseif(Node_status(node,i) == 1 && channel_s(node,i) == 0 && bck_of(node) == 1 &&  send_frame(node) == 0)
                if node == 1
                    if cw1 > 0
                        cw1 = cw1 - 1;
                    end
                    if cw1 == 0
                        send_frame(1) = 1;
                        bck_of(node) = 0;
                        back_off_finish(node) = 1;
                        Node_status(1,i+1:i+rts_slot+1) = 1;
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
                        Node_status(2,i+1:i+rts_slot+1) = 1;
                        send_either_frame = send_frame(1) || send_frame(2);
                    end
                end
                if send_either_frame == 1
                    if send_frame(1)==1
                        Node_status(1,i:i+rts_slot+sifs_slot+cts_slot+1) = 1;
                    end
                    if send_frame(2)==1
                        Node_status(2,i:i+rts_slot+sifs_slot+cts_slot+1) = 1;
                    end
                end
            elseif(Node_status(node,i) == 1 && bck_of(node) == 0 && back_off_finish(node) == 1 && send_frame(node) == 1)
                channel_s(node,i:i+rts_slot+sifs_slot+cts_slot) = 1;
                back_off_finish(node) = 0;
                NAV(node,i) = rts_slot+sifs_slot+cts_slot;
                NAV(node,i+1) = NAV(node,i) - 1;
                Node_status(node,i+1 : i+rts_slot) = 1;
                send_frame(node) = 0;
            elseif channel_s(node,i) == 1 && NAV(node,i) > sifs_slot+cts_slot
                NAV(node,i+1) = NAV(node,i) - 1;
                if NAV(node,i+1) == sifs_slot+cts_slot
                    if Node_status(1,i) == 1 && node == 1
                        Node_status(3,i:i+sifs_slot+cts_slot) = 1;
                        Node_status(1,i:sim_slot) = 0;
                    elseif Node_status(2,i) == 1 && node == 2
                        Node_status(4,i:i+sifs_slot+cts_slot) = 1;
                    end
                end
            elseif channel_s(node,i) == 1 && (Node_status(3,i) == 1 || Node_status(4,i) == 1) && NAV(node,i) > cts_slot
                NAV(node,i+1) = NAV(node,i) - 1;
            elseif channel_s(node,i) == 1 && (Node_status(3,i) == 1 || Node_status(4,i) == 1) && NAV(node,i) == cts_slot
                if no_CTS(node) == 1
                    NAV(node,i+1) = NAV(node,i) - 2;
                    no_CTS(node) = 0;
                else
                    sendCts = 1;
                    NAV(node,i+1) = NAV(node,i) - 1;
                end
            elseif (channel_s(node,i) == 1 && (Node_status(3,i) == 1 || Node_status(4,i) == 1) && NAV(node,i) == 1)
                if Node_status(3,i) == 1
                    if sendCts == 1
                        Traffic_A_Received(s1) = 1;
                        sendCts = 0;
                        send_either_frame = 0;
                        send_frame(1)  = 0;
                        Node_status(1,i:sim_slot) = 0;
                        num_success_1 = num_success_1+1;
                        i = i+sifs_slot+frame_slot+sifs_slot+ack_slot;
                        collision_flag_1 = 0;
                    end
                elseif Node_status(4,i) == 1
                    if sendCts == 1
                        Traffic_C_Received(s2) = 1;
                        sendCts = 0;
                        send_either_frame = 0;
                        send_frame(2)  = 0;
                        num_success_2 = num_success_2 + 1;
                        i = i+sifs_slot+frame_slot+sifs_slot+ack_slot;
                        collision_flag_2 = 0;
                    end
                end
                if i > sim_slot
                    break;
                end
                NAV(node,i) = NAV(node,i) - 1;
            end
            if Node_status(3,i) ==0 && Node_status(4,i) ==0
                if channel_s(1,i)==1 && channel_s(2,i)==1
                    if no_CTS(1) == 0 && no_CTS(2) == 0
                        no_CTS(1) = 1;
                        no_CTS(2) = 1;
                        num_of_collision(1) = num_of_collision(1) + 1;
                        num_of_collision(2) = num_of_collision(2) + 1;
                        collision_flag_1=collision_flag_1+1;
                        collision_flag_2=collision_flag_2+1;
                    end
                end
            elseif Node_status(3,i) == 1 && Node_status(4,i) ==0
                if channel_s(1,i)== 1 && channel_s(2,i)== 1
                    if sendCts == 1
                        if no_CTS(2) == 0
                            no_CTS(2) = 1;
                            num_of_collision(2) = num_of_collision(2) + 1;
                            collision_flag_2=collision_flag_2+1;
                        end
                    end
                end
            elseif Node_status(3,i) == 0 && Node_status(4,i) == 1
                if channel_s(1,i)== 1 && channel_s(2,i)== 1
                    if sendCts == 1
                        if no_CTS(1)
                            no_CTS(1) = 1;
                            num_of_collision(1) = num_of_collision(1) + 1;
                            collision_flag_1=collision_flag_1+1;
                        end
                    end
                end
            end
        end
    end
    
    Throughput1 = (num_success_1)*1500*8/10/1000;
    Throughput2 = (num_success_2)*1500*8/10/1000;
    FI = num_success_1/num_success_2;
    output_data=[Throughput1 Throughput2 num_of_collision(1) num_of_collision(2) FI];
    fprintf("********HID VS***************\n");
    fprintf('Frames in A (%d) = %d Frames in C (%d) = %d\n',Arrv_rate_A,a(2),Arrv_rate_C,c(2));
    fprintf('Success in A (%d) = %d Success in C (%d) = %d\n',Arrv_rate_A,num_success_1,Arrv_rate_C,num_success_2);
    fprintf('collisions in A (%d) = %d collisions in C (%d) = %d\n',Arrv_rate_A,num_of_collision(1),Arrv_rate_C,num_of_collision(2));
end