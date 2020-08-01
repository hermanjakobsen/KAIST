function [paths] = get_paths(folder)
    s = dir(folder);
    paths = (strcat(folder, '/', {s(~[s.isdir]).name}));
end