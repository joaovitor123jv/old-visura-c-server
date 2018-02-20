CREATE USER 'interface'@'localhost' IDENTIFIED BY 'essaSenhaEsoPraInteFaCeSaBeRPorQuESiM';
GRANT SELECT, UPDATE, DELETE, INSERT ON teste.* TO 'interface'@'localhost';
FLUSH PRIVILEGES;
