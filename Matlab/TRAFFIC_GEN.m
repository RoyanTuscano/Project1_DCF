function Frame_Arrv_slot_array = TRAFFIC_GEN(Arrv_rate)

    global slot_time;
    slot_time = 0.000020;
    sim_tim=10;

    Num_of_slots = sim_tim / slot_time;

    curr_slot = 0;      

    while curr_slot < Num_of_slots

        time_Int = -1/Arrv_rate * log(1-rand(1,1));

        slot_Int = ceil( time_Int / slot_time );
        if curr_slot==0
            Frame_Arrv_slot_array=[slot_Int];
        end
        Frame_Arrv_slot_array = [Frame_Arrv_slot_array Frame_Arrv_slot_array(end)+slot_Int]; 
                    
        curr_slot = curr_slot + slot_Int;        

    end 

    Frame_Arrv_slot_array(end) = [];

end