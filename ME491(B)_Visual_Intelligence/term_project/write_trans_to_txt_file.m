function write_trans_to_txt_file(T_all,filename) 
    path = fullfile('evalutationCode',filename);
    file_id = fopen(path,'w');
    T_prev = [1 0 0 0; 0 1 0 0; 0 0 1 0; 0 0 0 1];
    for i = 1:numel(T_all)
        T = T_all{i};
        T(4,:) = [0 0 0 1];
        T_curr = T_prev*T;
        T_prev = T_curr;
        T_curr = T_curr(1:3,:);
        T_rows = reshape(T_curr',1,[]);
        fprintf(file_id,'%f %f %f %f %f %f %f %f %f %f %f %f',T_rows);
        fprintf(file_id,'\n');
        
    end
    fclose(file_id);
end