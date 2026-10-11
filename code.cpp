#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int main() {
    int D, n;
    cin >> D >> n;
    
    // Calculate number of leaf nodes
    int leaf_count = 1 << (D - 1);  // 2^(D-1)
    
    // Initialize all switches to closed (0)
    vector<bool> switches((1 << D) - 1, false);  // 2^D - 1 switches
    
    // Simulate n balls falling
    int pos = 0;
    for (int ball = 0; ball < n; ball++) {
        pos = 0;  // Start at root (index 0)
        
        // Traverse from root to leaf
        for (int depth = 0; depth < D - 1; depth++) {
            if (!switches[pos]) {
                // Switch is closed, open it and go left
                switches[pos] = true;
                pos = 2 * pos + 1;  // Left child
            } else {
                // Switch is open, close it and go right
                switches[pos] = false;
                pos = 2 * pos + 2;  // Right child
            }
        }
        // pos now points to the leaf node index
    }
    
    // Output the final position (1-indexed)
    cout << pos + 1 << endl;
    
    // Output leaf node switch states (from left to right)
    for (int i = 0; i < leaf_count; i++) {
        if (i > 0) cout << " ";
        cout << switches[leaf_count - 1 + i];
    }
    cout << endl;
    
    return 0;
}