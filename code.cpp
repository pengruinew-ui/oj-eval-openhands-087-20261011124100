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
    vector<bool> switch_states((1 << D) - 1, false);
    
    // For each switch in the tree
    for (int i = 0; i < (1 << D) - 1; i++) {
        // Calculate the depth of this switch
        int depth = 0;
        int temp = i;
        while (temp > 0) {
            temp = (temp - 1) / 2;
            depth++;
        }
        
        // For depth d, the toggle pattern has period 2^(d+1)
        // and the switch is toggled for the first 2^d steps of each period
        int period = 1 << (depth + 1);  // 2^(depth+1)
        int half_period = 1 << depth;   // 2^depth
        
        // Calculate how many times this switch is toggled by n balls
        int full_cycles = n / period;
        int remainder = n % period;
        
        // Each full cycle contributes half_period toggles
        int toggle_count = full_cycles * half_period;
        
        // Add toggles from the incomplete cycle
        // The switch is toggled for the first half_period steps of the period
        if (remainder > half_period) {
            toggle_count += half_period;
        } else {
            toggle_count += remainder;
        }
        
        // Switch is open (1) if toggled odd number of times
        switch_states[i] = (toggle_count % 2 == 1);
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