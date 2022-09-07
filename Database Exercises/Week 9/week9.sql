
---- [6.4.6] ----
-- a) Find the average speed of pcs.
SELECT AVG(speed) FROM PC

-- b) Find the average speed of laptops costing over $1000
SELECT AVG(speed) FROM laptops WHERE price > 1000

-- d) Find the average price of PC's and laptops made by manufacturer D
SELECT AVG(price) FROM (SELECT price, maker FROM pc JOIN product ON pc.model = product.model UNION SELECT price, maker FROM laptop JOIN product ON laptop.model = product.model) WHERE maker = "D";

-- e) Find for each different speed, the average price of a PC
SELECT speed, AVG(price) FROM PC GROUP BY speed

-- f) Find for each manufacture the average screen size of its laptops
SELECT AVG(screen), maker FROM Laptop, product WHERE Laptop.model = product.model GROUP BY maker;

-- g) Find the manufacturer that make at least three different models of PC.
SELECT maker FROM product JOIN pc on pc.model = product.model GROUP BY maker HAVING count(maker) >= 3;

-- h) Find for each manufacturer who sells PC's the maimum price of a PC.
SELECT MAX(price), maker FROM pc JOIN product ON pc.model = product.model GROUP by maker;

-- i) Find, for each speed of PC above 2.0, the average price.
SELECT AVG(price), speed FROM pc WHERE speed > 2 GROUP BY speed;


---- [6.4.7] ----
-- a) Find the number of battleship classes.
SELECT COUNT(class) FROM (SELECT DISTINCT class FROM ships UNION SELECT DISTINCT class FROM classes);

-- d) Find for each class the year in which the first ship of that class was launched.
SELECT MIN(launched) FROM Ships GROUP BY class;

-- e) Find for each class the number of ships of that class sunk in battle.
SELECT COUNT(class) FROM Outcomes JOIN Ships ON Ships.name = Outcomes.ship WHERE result = "sunk" GROUP BY class;