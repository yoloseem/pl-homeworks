getR rows cols i = i `div` (cols + 1)
getC rows cols i = i `mod` (cols + 1)
getI rows cols r c = (cols + 1) * r + c

minecount :: Int -> Int -> Int -> String -> Int
minecount rows cols i game
    | r < 0 = 0
    | r >= rows = 0
    | c < 0 = 0
    | c >= cols = 0
    | game !! i == '*' = 1
    | otherwise = 0
    where r = getR rows cols i
          c = getC rows cols i

minesweep' :: Int -> Int -> Int -> String -> Char
minesweep' rows cols i game
    | game !! i == '*' = '*'
    | game !! i == '\\' = '\\'
    | otherwise = ['0'..'9'] !! (
        (minecount rows cols (getI rows cols (r - 1) (c - 1)) game) +
        (minecount rows cols (getI rows cols (r - 1) c) game) +
        (minecount rows cols (getI rows cols (r - 1) (c + 1)) game) +
        (minecount rows cols (getI rows cols r (c - 1)) game) +
        (minecount rows cols (getI rows cols r (c + 1)) game) +
        (minecount rows cols (getI rows cols (r + 1) (c - 1)) game) +
        (minecount rows cols (getI rows cols (r + 1) c) game) +
        (minecount rows cols (getI rows cols (r + 1) (c + 1)) game)
    )
    where r = getR rows cols i
          c = getC rows cols i

minesweep :: Int -> Int -> String -> String
minesweep r c game = map (\i -> minesweep' r c i game) [0..(length game - 1)]
