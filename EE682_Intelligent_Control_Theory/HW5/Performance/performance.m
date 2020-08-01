clear all
close all
clc 

data_MOPBIL = load('../MOPBIL/Results/2_Nondom_Result_PBIL.txt');
data_MQEA = load('../MQEA/Results/2_MQEA_nondom_fitness.txt');

rowsize_MOPBIL = length(data_MOPBIL(:,1));
colsize_MOPBIL = length(data_MOPBIL(1,:));

rowsize_MQEA = length(data_MQEA(:,1));
colsize_MQEA = length(data_MQEA(1,:));

%%%%%%%%%%%%%%%%%%%%%%% Diversity Metric %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

diversity_MOPBIL = Fdiversity(data_MOPBIL,rowsize_MOPBIL,colsize_MOPBIL)
diversity_MQEA   = Fdiversity(data_MQEA,rowsize_MQEA,colsize_MQEA)

%%%%%%%%%%%%%%%%%%%%%%%%%%% Hypervolume %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
reference_size = 10;

for i = 1:colsize_MOPBIL
    reference_MOPBIL(i) = reference_size;
end

for i = 1:colsize_MQEA
    reference_MQEA(i) = reference_size;
end

iteration_num = 100000;
hyper_MOPBIL = Fhypervolume(data_MOPBIL(1:10,:),reference_MOPBIL,iteration_num) * reference_size^colsize_MOPBIL
hyper_MQEA = Fhypervolume(data_MQEA(1:10,:),reference_MQEA,iteration_num) * reference_size^colsize_MQEA
