-- MySQL Script generated by MySQL Workbench
-- qua 21 jun 2017 22:21:39 -03
-- Model: New Model    Version: 1.0
-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema mydb
-- -----------------------------------------------------
-- -----------------------------------------------------
-- Schema teste
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema teste
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `teste` DEFAULT CHARACTER SET utf8mb4 ;
USE `teste` ;

-- -----------------------------------------------------
-- Table `teste`.`estado`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `teste`.`estado` (
  `idestado` INT(11) NOT NULL AUTO_INCREMENT,
  `nome` VARCHAR(2) NOT NULL,
  PRIMARY KEY (`idestado`),
  UNIQUE INDEX `nome_UNIQUE` (`nome` ASC))
ENGINE = InnoDB
AUTO_INCREMENT = 50
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `teste`.`cidade`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `teste`.`cidade` (
  `idcidade` INT(11) NOT NULL AUTO_INCREMENT,
  `nome` VARCHAR(58) NOT NULL COMMENT 'A cidade com o maior nome do mundo possui 58 letras',
  `quantidadeDeHabitantes` INT(11) NULL DEFAULT NULL,
  `estado_idestado` INT(11) NOT NULL,
  PRIMARY KEY (`idcidade`),
  INDEX `fk_cidade_estado1_idx` (`estado_idestado` ASC),
  CONSTRAINT `fk_cidade_estado1`
    FOREIGN KEY (`estado_idestado`)
    REFERENCES `teste`.`estado` (`idestado`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 41
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `teste`.`localizacao`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `teste`.`localizacao` (
  `idlocalizacao` INT(11) NOT NULL AUTO_INCREMENT,
  `cep` VARCHAR(8) NOT NULL,
  `rua` VARCHAR(45) NULL DEFAULT NULL,
  `complemento` VARCHAR(160) NULL DEFAULT NULL,
  `bairro` VARCHAR(58) NULL DEFAULT NULL,
  `numero` INT(11) NULL DEFAULT NULL,
  `cidade_idcidade` INT(11) NOT NULL,
  PRIMARY KEY (`idlocalizacao`),
  INDEX `fk_localizacao_cidade1_idx` (`cidade_idcidade` ASC),
  CONSTRAINT `fk_localizacao_cidade1`
    FOREIGN KEY (`cidade_idcidade`)
    REFERENCES `teste`.`cidade` (`idcidade`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 7
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `teste`.`cliente`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `teste`.`cliente` (
  `idcliente` INT(11) NOT NULL AUTO_INCREMENT,
  `nome` VARCHAR(64) NOT NULL,
  `sobrenome` VARCHAR(64) NOT NULL,
  `sexo` ENUM('1', '2') NOT NULL COMMENT '1 para Homem, 2 para mulher',
  `telefone` VARCHAR(11) NULL DEFAULT NULL COMMENT 'Dois primeiros caracteres (Código de Área)\nDemais caracteres, numero em si',
  `email` VARCHAR(64) NOT NULL,
  `datanascimento` DATE NOT NULL,
  `localizacao_idlocalizacao` INT(11) NULL DEFAULT NULL,
  `senha` VARCHAR(256) NOT NULL COMMENT 'Senha criptografada com RSA',
  PRIMARY KEY (`idcliente`),
  INDEX `fk_cliente_localizacao1_idx` (`localizacao_idlocalizacao` ASC),
  CONSTRAINT `fk_cliente_localizacao1`
    FOREIGN KEY (`localizacao_idlocalizacao`)
    REFERENCES `teste`.`localizacao` (`idlocalizacao`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `teste`.`contratante`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `teste`.`contratante` (
  `idcontratante` INT(11) NOT NULL AUTO_INCREMENT,
  `nome` VARCHAR(100) NOT NULL,
  `cnpj` VARCHAR(14) NOT NULL,
  `plano` INT(2) NOT NULL COMMENT 'Sendo 1, o plano com menos recursos, e 99 o plano com mais recursos',
  `email` VARCHAR(64) NOT NULL,
  `telefone` VARCHAR(11) NULL DEFAULT NULL COMMENT 'Dois primeiros Números: Código de área\nDemais números, telefone em si',
  `localizacao_idlocalizacao` INT(11) NOT NULL,
  PRIMARY KEY (`idcontratante`),
  UNIQUE INDEX `email_UNIQUE` (`email` ASC),
  UNIQUE INDEX `cnpj_UNIQUE` (`cnpj` ASC),
  INDEX `fk_contratante_localizacao1_idx` (`localizacao_idlocalizacao` ASC),
  CONSTRAINT `fk_contratante_localizacao1`
    FOREIGN KEY (`localizacao_idlocalizacao`)
    REFERENCES `teste`.`localizacao` (`idlocalizacao`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `teste`.`produto`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `teste`.`produto` (
  `idproduto` INT(11) NOT NULL AUTO_INCREMENT,
  `contratante_idcontratante` INT(11) NOT NULL,
  `cliques` INT(11) NULL DEFAULT NULL COMMENT 'Quantidade de cliques que o produto recebeu',
  `nomeProduto` VARCHAR(100) NULL DEFAULT NULL,
  `idficticio` VARCHAR(10) NOT NULL COMMENT 'ID que a empresa terá conhecimento (fora do banco)',
  PRIMARY KEY (`idproduto`),
  INDEX `fk_produto_contratante_idx` (`contratante_idcontratante` ASC),
  INDEX `indice_idFicticio` (`idficticio` ASC),
  CONSTRAINT `fk_produto_contratante`
    FOREIGN KEY (`contratante_idcontratante`)
    REFERENCES `teste`.`contratante` (`idcontratante`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
