/*
    Magic Square Generator
    Input: Odd number n (3 <= n <= 15)
    Output: n x n Magic Square
*/

void main()
{
    int n;
    int i, j;
    int row, col;
    int num, limit;
    int prev_row, prev_col;
    int square[225]; // 15 * 15

    while (1) {
        read(n);
        if (n == 0) break;
        if (n % 2 == 0) continue;

        // Initialize square
        i = 0;
        while (i < 225) {
            square[i] = 0;
            ++i;
        }

        row = 0;
        col = n / 2;
        num = 1;
        limit = n * n;

        while (num <= limit) {
            square[row * 15 + col] = num;
            ++num;

            prev_row = row;
            prev_col = col;

            --row;
            ++col;

            if (row < 0) row = n - 1;
            if (col >= n) col = 0;

            if (square[row * 15 + col] != 0) {
                row = prev_row + 1;
                if (row == n) row = 0;
                col = prev_col;
            }
        }

        // Print square
        i = 0;
        while (i < n) {
            j = 0;
            while (j < n) {
                write(square[i * 15 + j]);
                ++j;
            }
            lf();
            ++i;
        }
    }
}
