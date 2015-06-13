flipCoin' :: String -> Integer -> [Integer]
flipCoin' [] o = [0]
flipCoin' [x] o
    | x == 'H' = [0]
    | otherwise = [o + 1, 0]
flipCoin' (x:xs) o
    | x == y = flipCoin' xs (o + 1)
    | otherwise = [o + 1] ++ flipCoin' xs (o + 1)
    where y = head xs


flipCoin :: String -> [Integer]
flipCoin xs = flipCoin' xs 0
