-- MySQL Script generated by MySQL Workbench
-- 2017年10月14日 03時30分35秒
-- Model: New Model    Version: 1.0
-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema teste
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema teste
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `teste` DEFAULT CHARACTER SET utf8 ;
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
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `teste`.`cidade`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `teste`.`cidade` (
  `idcidade` INT(11) NOT NULL AUTO_INCREMENT,
  `nome` VARCHAR(58) NOT NULL,
  `quantidadeDeHabitantes` INT(11) NULL DEFAULT NULL,
  `estado_idestado` INT(11) NOT NULL,
  PRIMARY KEY (`idcidade`),
  INDEX `fk_cidade_estado_idx` (`estado_idestado` ASC),
  CONSTRAINT `fk_cidade_estado`
    FOREIGN KEY (`estado_idestado`)
    REFERENCES `teste`.`estado` (`idestado`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
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
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `teste`.`contratante`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `teste`.`contratante` (
  `idcontratante` INT(11) NOT NULL AUTO_INCREMENT,
  `nome` VARCHAR(100) NOT NULL,
  `cnpj` VARCHAR(14) NOT NULL,
  `plano` INT(2) NOT NULL DEFAULT '0',
  `email` VARCHAR(64) NOT NULL,
  `telefone` VARCHAR(11) NULL DEFAULT NULL,
  `senha` BLOB NOT NULL,
  `localizacao_idlocalizacao` INT(11) NOT NULL,
  PRIMARY KEY (`idcontratante`),
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
  `idproduto` VARCHAR(10) NOT NULL,
  `visualizacoes` INT(11) NOT NULL DEFAULT 0,
  `nomeProduto` VARCHAR(100) NULL DEFAULT NULL,
  `duracao` INT(2) NOT NULL DEFAULT 30,
  `datacriacao` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `visualizacoesunicas` INT(11) NOT NULL DEFAULT 0,
  `visualizacaoanom` INT(11) NOT NULL DEFAULT 0,
  `contratante_idcontratante` INT(11) NOT NULL,
  `descricao` VARCHAR(140) NULL DEFAULT NULL,
  `avaliacaoPositiva` INT NOT NULL DEFAULT 0,
  `avaliacaoNegativa` INT NOT NULL DEFAULT 0,
  PRIMARY KEY (`idproduto`),
  INDEX `fk_produto_contratante1_idx` (`contratante_idcontratante` ASC),
  CONSTRAINT `fk_produto_contratante1`
    FOREIGN KEY (`contratante_idcontratante`)
    REFERENCES `teste`.`contratante` (`idcontratante`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `teste`.`LogProduto`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `teste`.`LogProduto` (
  `idLogProduto` INT(11) NOT NULL AUTO_INCREMENT,
  `visualAnomAtual` INT(11) NOT NULL COMMENT 'Quantidade de Visualizacoes Atual SOMENTE\nTamanho do Log diminuido pela metade',
  `visualAtual` INT(11) NOT NULL,
  `dataGeracao` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Data de geração desse log',
  `produto_idproduto` VARCHAR(10) NOT NULL,
  PRIMARY KEY (`idLogProduto`),
  INDEX `fk_LogProduto_produto1_idx` (`produto_idproduto` ASC),
  INDEX `indiceReverso` USING BTREE (`idLogProduto` ASC, `produto_idproduto` ASC, `dataGeracao` ASC)  COMMENT 'Facilita procurando primeiro os indices mais novos',
  CONSTRAINT `fk_LogProduto_produto1`
    FOREIGN KEY (`produto_idproduto`)
    REFERENCES `teste`.`produto` (`idproduto`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `teste`.`cliente`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `teste`.`cliente` (
  `idcliente` INT(11) NOT NULL AUTO_INCREMENT,
  `nome` VARCHAR(64) NULL DEFAULT NULL,
  `sobrenome` VARCHAR(64) NULL DEFAULT NULL,
  `sexo` ENUM('1', '2', '3') NULL DEFAULT NULL COMMENT '1 = Masculino\n2 = Feminino\n3 = Outro',
  `telefone` VARCHAR(11) NULL DEFAULT NULL,
  `email` VARCHAR(64) NOT NULL,
  `datanascimento` DATE NULL DEFAULT NULL,
  `senha` BLOB NOT NULL,
  `localizacao_idlocalizacao` INT(11) NULL DEFAULT NULL,
  `pontos` INT(11) NOT NULL DEFAULT 0 COMMENT 'Já é inicializado com 0',
  PRIMARY KEY (`idcliente`, `email`),
  UNIQUE INDEX `email_UNIQUE` (`email` ASC),
  INDEX `fk_cliente_localizacao1_idx` (`localizacao_idlocalizacao` ASC),
  CONSTRAINT `fk_cliente_localizacao1`
    FOREIGN KEY (`localizacao_idlocalizacao`)
    REFERENCES `teste`.`localizacao` (`idlocalizacao`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `teste`.`visualizacaoDeUsuario`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `teste`.`visualizacaoDeUsuario` (
  `idvisualizacoes` INT(11) NOT NULL AUTO_INCREMENT,
  `quantidade` INT(11) NOT NULL DEFAULT '0',
  `cliente_idcliente` INT(11) NOT NULL,
  `produto_idproduto` VARCHAR(10) NOT NULL,
  `avaliacaoDoUsuario` TINYINT(4) NULL DEFAULT '0' COMMENT 'tinyint vai de -127 a 127\nAvaliação vai ser poucos bytes\n\nAvaliação Positiva: 1 (true)\nAvaliação Negativa: 0 (false)',
  PRIMARY KEY (`idvisualizacoes`),
  INDEX `fk_visualizacaoDeUsuario_cliente1_idx` (`cliente_idcliente` ASC),
  INDEX `fk_visualizacaoDeUsuario_produto1_idx` (`produto_idproduto` ASC),
  CONSTRAINT `fk_visualizacaoDeUsuario_cliente1`
    FOREIGN KEY (`cliente_idcliente`)
    REFERENCES `teste`.`cliente` (`idcliente`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_visualizacaoDeUsuario_produto1`
    FOREIGN KEY (`produto_idproduto`)
    REFERENCES `teste`.`produto` (`idproduto`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
