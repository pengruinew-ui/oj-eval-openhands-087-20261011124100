#include <iostream>
#include <vector>
using namespace std;

int main() {
    int D, n;
    cin >> D >> n;
    
    // Calculate number of leaf nodes
    int leaf_count = 1 << (D - 1);  // 2^(D-1)
    
    // For the final position: the path of the n-th ball is determined by 
    // the binary representation of (n-1) with D-1 bits
    // Each bit represents a decision: 0=left, 1=right
    int pos = 0;
    int path = n - 1;  // 0-indexed path
    
    // Calculate the final position by following the path
    for (int depth = 0; depth < D - 1; depth++) {
        if (path & 1) {
            // Right child
            pos = 2 * pos + 2;
        } else {
            // Left child
            pos = 2 * pos + 1;
        }
        path >>= 1;
    }
    
    // Output the final position (1-indexed)
    cout << pos + 1 << endl;
    
    // Now calculate the final switch states efficiently
    // For each switch, we can calculate how many times it's been toggled
    // by n balls using a mathematical approach rather than simulation
    
    // The key insight: each switch at depth d toggles in a pattern
    // with period 2^(d+1), where it's active for 2^d steps
    // For each node in the tree (all are switches)
    // Total nodes: 2^D - 1
    vector<bool> switch_states((1 << D) - 1, false);
    
    for (int i = 0; i < (1 << D) - 1; i++) {
        // Calculate the depth of node i
        // Depth 0: node 0
        // Depth 1: nodes 1,2
        // Depth 2: nodes 3,4,5,6
        // etc.
        int depth = 0;
        int nodes_at_depth = 1;  // Number of nodes at current depth
        int first_node_at_depth = 0;  // First node index at current depth
        
        while (first_node_at_depth + nodes_at_depth <= i) {
            depth++;
            first_node_at_depth += nodes_at_depth;
            nodes_at_depth *= 2;
        }
        
        // For a node at depth d, the number of balls that pass through it
        // follows a pattern based on binary representation
        // The node is visited by a ball if the ball's path matches the
        // path to this node for the first d steps
        
        // Calculate how many balls pass through this node
        // This is equivalent to counting numbers from 0 to n-1 that
        // have a specific prefix in their binary representation
        
        // The path to node i can be represented as a binary number
        // We need to find the prefix corresponding to the path to this node
        
        // Reconstruct the path to node i
        int path_to_node = 0;
        int bit_count = 0;
        int node = i;
        
        // Traverse from node i up to root, recording the path
        while (node > 0) {
            if (node % 2 == 1) {
                // Left child
                path_to_node |= (0 << bit_count);  // 0 for left
            } else {
                // Right child
                path_to_node |= (1 << bit_count);  // 1 for right
            }
            node = (node - 1) / 2;
            bit_count++;
        }
        
        // The prefix length is depth
        // We need to count how many numbers in [0, n-1] have the first
        // 'depth' bits equal to 'path_to_node'
        
        if (depth == 0) {
            // Root node - all balls pass through
            switch_states[i] = (n % 2 == 1);
        } else {
            // Calculate the number of balls with the required prefix
            // The numbers with the prefix are in the range:
            // [path_to_node << (D-1-depth), (path_to_node + 1) << (D-1-depth))
            // but we need to intersect with [0, n)
            
            int shift = D - 1 - depth;
            if (shift < 0) shift = 0;  // For leaf nodes
            
            int start = path_to_node << shift;
            int end = (path_to_node + 1) << shift;
            
            int count = 0;
            if (start < n) {
                count = min(n, end) - start;
            }
            
            // Switch is open if toggled odd number of times
            switch_states[i] = (count % 2 == 1);
        }
    }
    
    // Output leaf node switch states (from left to right)
    // Leaf nodes start at index (1 << (D-1)) - 1
    int leaf_start = (1 << (D-1)) - 1;
    for (int i = 0; i < leaf_count; i++) {
        if (i > 0) cout << " ";
        cout << switch_states[leaf_start + i];
    }
    cout << endl;
    
    return 0;
}