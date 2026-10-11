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
    // Each switch at position i will be toggled once for each ball 
    // that passes through it. We can calculate this by simulating 
    // all n balls but just tracking the toggle count for each switch
    vector<int> toggle_count((1 << D) - 1, 0);
    
    for (int ball = 1; ball <= n; ball++) {
        int current = 0;  // Start at root
        int ball_path = ball - 1;  // 0-indexed
        
        // Traverse from root to leaf
        for (int depth = 0; depth < D - 1; depth++) {
            toggle_count[current]++;
            
            if (ball_path & 1) {
                // Go right
                current = 2 * current + 2;
            } else {
                // Go left
                current = 2 * current + 1;
            }
            ball_path >>= 1;
        }
    }
    
    // Output leaf node switch states (from left to right)
    // Leaf nodes start at index (1 << (D-1)) - 1
    int leaf_start = (1 << (D-1)) - 1;
    for (int i = 0; i < leaf_count; i++) {
        if (i > 0) cout << " ";
        // Switch is open (1) if toggled odd number of times, closed (0) if even
        cout << (toggle_count[leaf_start + i] % 2);
    }
    cout << endl;
    
    return 0;
}