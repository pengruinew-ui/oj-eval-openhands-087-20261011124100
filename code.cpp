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
    
    // Now calculate the final switch states
    // We can use the fact that the toggle pattern is deterministic
    // Each switch at depth d is toggled in a pattern with period 2^(d+1)
    
    // Total number of nodes (switches) in a complete binary tree of depth D
    int total_nodes = (1 << D) - 1;
    vector<bool> switch_states(total_nodes, false);
    
    // For each node, calculate how many times it's been toggled
    for (int i = 0; i < total_nodes; i++) {
        // Calculate the depth of node i
        int depth = 0;
        int temp = i;
        while (temp) {
            temp = (temp - 1) / 2;
            depth++;
        }
        
        // For a node at depth d, the number of times it's toggled by n balls
        // is equal to the number of balls that pass through it
        // This can be calculated as: (n + offset) >> (depth + 1) << depth + min((n + offset) & ((1 << (depth + 1)) - 1), 1 << depth)
        // But there's a simpler way: the number of balls that pass through
        // a node at depth d is floor((n + (1 << depth) - 1) / (1 << (depth + 1))) * (1 << depth) + 
        //                          max(0, (n + (1 << depth) - 1) % (1 << (depth + 1)) - (1 << depth) + 1)
        
        // Simpler approach: the number of balls that pass through node i
        // is the number of integers k in [0, n-1] such that the first 'depth' bits
        // of k match the path to node i
        
        // Reconstruct the path to node i as a binary number
        int node_path = 0;
        int bit_pos = 0;
        int node = i;
        while (node > 0) {
            if (node % 2 == 0) {  // Right child
                node_path |= (1 << bit_pos);
            }
            // Left child (bit remains 0)
            node = (node - 1) / 2;
            bit_pos++;
        }
        
        // Count how many numbers in [0, n-1] have the same first 'depth' bits
        // as node_path
        if (depth == 0) {
            // Root node - all n balls pass through
            switch_states[i] = (n % 2 == 1);
        } else {
            // The numbers with the same prefix are in the range:
            // [node_path << (D-1-depth), (node_path + 1) << (D-1-depth))
            // Intersect with [0, n)
            int shift = D - 1 - depth;
            if (shift < 0) shift = 0;
            
            int start = node_path << shift;
            int end = (node_path + 1) << shift;
            
            int count = 0;
            if (start < n) {
                count = min(n, end) - start;
            }
            switch_states[i] = (count % 2 == 1);
        }
    }
    
    // Output leaf node switch states (from left to right)
    int leaf_start = (1 << (D-1)) - 1;
    for (int i = 0; i < leaf_count; i++) {
        if (i > 0) cout << " ";
        cout << switch_states[leaf_start + i];
    }
    cout << endl;
    
    return 0;
}