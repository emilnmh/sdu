CREATE TABLE Classes (
  class VARCHAR,
  type CHAR(2),
  country VARCHAR,
  numGuns INT,
  bore INT,
  displacement INT,
  PRIMARY KEY(class)
);

INSERT INTO Classes (class, type, country, numGuns, bore, displacement)
VALUES ('Bismarck', 'bb', 'Germany', 8, 15, 42000), ('Iowa', 'bb', 'USA', 9, 16, 46000), ('Kongo', 'be', 'Japan', 8, 14, 32000),
        ('North Carolina', 'bb', 'USA', 9, 16, 37000), ('Renown', 'be', 'Gt. Britain', 6, 15, 32000),
        ('Revenge', 'bb', 'Gt. Britain', 8, 15, 29000), ('Tennessee', 'bb', 'USA', 12, 14, 32000),
        ('Yamato', 'bb', 'Japan', 9, 18, 65000);

CREATE TABLE Battles (
  name VARCHAR,
  date DATE,
  PRIMARY KEY(name)
);

INSERT INTO Battles (name, date)
VALUES ('Denmark Strait', '1941-05-24'), ('Guadalcanal', '1942-11-15'), ('North Cape', '1943-12-26'), ('Surigao Strait', '1944-10-25');

CREATE TABLE Outcomes (
  ship VARCHAR,
  battle VARCHAR,
  result VARCHAR,
  PRIMARY KEY(ship, battle)
);

INSERT INTO Outcomes (ship, battle, result)
VALUES ('Arizona', 'Pearl Harbor', 'sunk'), ('Bismarck', 'Denmark Strait', 'sunk'), ('California', 'Surigao Strait', 'ok'),
        ('Duke of York', 'North Cape', 'ok'), ('Fuso', 'Surigao Strait', 'sunk'), ('Hood', 'Denmark Strait', 'sunk'),
        ('King George V', 'Denmark Strait', 'ok'), ('Kirishima', 'Guadalcanal', 'sunk'), ('Prince of Wales', 'Denmark Strait', 'damaged'),
        ('Rodney', 'Denmark Strait', 'ok'), ('Scharnhorst', 'North Cape', 'sunk'), ('South Dakota', 'Guadalcanal', 'damaged'),
        ('Tennessee', 'Surigao Strait', 'ok'), ('Washington', 'Guadalcanal', 'ok'), ('West Virginia', 'Surigao Strait', 'ok'),
        ('Yamashiro', 'Surigao Strait', 'sunk');

CREATE TABLE Ships (
  name VARCHAR,
  class VARCHAR,
  launched INT,
  PRIMARY KEY(name)
);

INSERT INTO Ships (name, class, launched)
VALUES ('California', 'Tennessee', 1921), ('Haruna', 'Kongo', 1915), ('Hiei', 'Kongo', 1914), ('Iowa', 'Iowa', 1943),
        ('Kirishima', 'Kongo', 1915), ('Kongo', 'Kongo', 1913), ('Missouri', 'Iowa', 1944), ('Musashi', 'Yamato', 1942),
        ('New Jersey', 'Iowa', 1943), ('North Carolina', 'North Carolina', 1941), ('Ramillies', 'Revenge', 1917),
        ('Renown', 'Renown', 1916), ('Repulse', 'Renown', 1916), ('Resolution', 'Revenge', 1916), ('Revenge', 'Revenge', 1916),
        ('Royal Oak', 'Revenge', 1916), ('Royal Sovereign', 'Revenge', 1916), ('Tennessee', 'Tennessee', 1920),
        ('Washington', 'North Carolina', 1941), ('Wisconsin', 'Iowa', 1944), ('Yamato', 'Yamato', 1941);