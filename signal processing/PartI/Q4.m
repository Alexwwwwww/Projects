% the variable graph encodes the network 
graph = ...
    [ 0 2 4 inf*ones(1,7);
    2 0 1 inf inf 2 inf inf 4 4;
    4 1 0 4 2 2 inf inf inf inf;
    inf inf 4 0 inf*ones(1,6);
    inf inf 2 inf 0 inf 1 inf inf inf;
    inf 2 2 inf inf 0 4 4 3 inf;
    inf inf inf inf 1 4 0 1 inf inf;
    inf*ones(1,5) 4 1 0 inf inf;
    inf 4 inf inf inf 3 inf inf 0 inf;
    inf 4 inf*ones(1,7) 0];

% number of nodes
n_nodes = size(graph,1); 

% set the source node
source = 6; 

% est_cost is a n_nodes by 1 matrix containing the current estimates of
% the path cost from source to every other node.
% We initialize it using the direct connections
est_cost = graph(:,source);

% find the indices of the neighbors of the source node
index = (~isinf(graph(:,source)));

% predecessor is a n_nodes by 1 matrix containing the predecessor node for each destination
% We initialize it so that it is zero if the predecessor is unknown and equal to
% the source node for the neighbors of the source node
predecessor = zeros(n_nodes, 1);
predecessor(index) = source;

% queue is a vector containing of the nodes to explore. 
% It is the complement of the set N' in the lecture notes.
% We initialize it so that it contains every node except for the source
queue = find( graph(:,source) > 0 );

% Run Dijkstra's algorithm

while ~isempty(queue)

    % Find the node, w, that is in the queue and has the smallest estimated path cost
    [~, id]= min(est_cost(queue));
    w = queue(id);

    % delete node w from the queue
    queue(id)= [];
      
    % % % % % Revise the following code % % % % %

    % for all nodes in queue and adjacent to w, 
    % if the estimated cost via w is lower than the current path estimate, 
    % then update the estimated cost and set the predecessor to w

    % % % % Do not change the code below % % % %

end

% Run back tracking to find the best path from the source 
% to each destination

% The columns of the n_nodes by n_nodes matrix path contain the nodes
% visited along the best path from source to each destination node
path = zeros(n_nodes);

% Loop over all possible destination nodes
for dest = 1:n_nodes
    % allocate memory for the inverse path (destination to source)
    invpath = zeros(n_nodes,1);

    % do backtracking
    prev = dest;
    i=1;
    while prev~=source && prev~=0
        % add the node to the path
        invpath(i) = prev;
        % move to the previous node
        prev = predecessor(prev);
        %
        i=i+1;
    end
    % check if there is a valid path
    if  prev ~= 0
        invpath(i) = prev;
        path(1:i,dest) = flip(invpath(1:i));
    end
end
        
% display results
disp('Destinations: ');
disp(1:length(graph));
disp(['Path from ' num2str(source) ' to any destination: ']);
disp(path)
disp('and the relative cost');
disp(est_cost')
