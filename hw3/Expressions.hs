import Data.List


prod :: [String] -> [String] -> [String]
prod xs ys = [x ++ y | x <- xs, y <- ys]


prodBoth :: [String] -> [String] -> [String]
prodBoth xs ys = (prod xs ys) ++ (prod ys xs)


prodExpr :: Int -> Int -> [String]
prodExpr n d = prod xs xs
    where xs = expr n d


expr :: Int -> Int -> [String]
expr n 1
    | n `mod` 2 == 0 = [concat $ replicate (n `div` 2) "()"]
    | otherwise = []
expr n d
    | n <= 1 = []
    | n `mod` 2 /= 0 = []
    | d <= 0 = []
    | d * 2 > n = []
    | otherwise = nub (
        (map (\e -> "(" ++ e ++ ")") (expr (n - 2) (d - 1))) ++
        (concatMap (\(m, b) -> prodBoth (expr m b) (expr (n - m) d)) mbs) ++
        (if n `mod` 2 == 0 then prodExpr (n `div` 2) d else [])
    )
    where mbs = [(m, b) | m <- [2,4..(n - 2)], b <- [1..d], n /= m && m >= b * 2 && (n - m) >= d * 2]


expression :: Int -> Int -> Int
expression n d = length $ expr n d
