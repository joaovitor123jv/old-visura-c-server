-- MySQL Script generated by MySQL Workbench
-- 2018年01月30日 14時01分15秒
-- Model: New Model    Version: 1.0
-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema mydb
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema mydb
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `mydb` DEFAULT CHARACTER SET utf8 ;
USE `mydb` ;

-- -----------------------------------------------------
-- Table `mydb`.`estado`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`estado` (
  `idestado` INT(11) NOT NULL AUTO_INCREMENT,
  `nome` VARCHAR(2) NOT NULL,
  PRIMARY KEY (`idestado`),
  UNIQUE INDEX `nome_UNIQUE` (`nome` ASC))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `mydb`.`cidade`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`cidade` (
  `idcidade` INT(11) NOT NULL AUTO_INCREMENT,
  `nome` VARCHAR(58) NOT NULL,
  `quantidadeDeHabitantes` INT(11) NULL DEFAULT NULL,
  `estado_idestado` INT(11) NOT NULL,
  PRIMARY KEY (`idcidade`),
  INDEX `fk_cidade_estado_idx` (`estado_idestado` ASC),
  CONSTRAINT `fk_cidade_estado`
    FOREIGN KEY (`estado_idestado`)
    REFERENCES `mydb`.`estado` (`idestado`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `mydb`.`localizacao`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`localizacao` (
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
    REFERENCES `mydb`.`cidade` (`idcidade`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `mydb`.`contratante`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`contratante` (
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
    REFERENCES `mydb`.`localizacao` (`idlocalizacao`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `mydb`.`produto`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`produto` (
  `idproduto` VARCHAR(10) NOT NULL,
  `visualizacoes` INT(11) NOT NULL DEFAULT '0',
  `nomeProduto` VARCHAR(100) NULL DEFAULT NULL,
  `duracao` INT(2) NOT NULL DEFAULT '30',
  `datacriacao` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `visualizacoesunicas` INT(11) NOT NULL DEFAULT '0',
  `visualizacaoanom` INT(11) NOT NULL DEFAULT '0',
  `contratante_idcontratante` INT(11) NOT NULL,
  `descricao` VARCHAR(140) NULL DEFAULT NULL,
  `avaliacaoPositiva` INT(11) NOT NULL DEFAULT '0',
  `avaliacaoNegativa` INT(11) NOT NULL DEFAULT '0',
  `tipo` ENUM('1', '2', '3') NOT NULL DEFAULT '1' COMMENT 'Sendo:\n	1 = RA\n	2 = RV\n	3 = RA/RV',
  `categoria` VARCHAR(2) NOT NULL DEFAULT '0' COMMENT 'Categoria do produto\n	Mercado\n	Compras\n	Social....\n\nNecessário criar tabelas',
  PRIMARY KEY (`idproduto`),
  INDEX `fk_produto_contratante1_idx` (`contratante_idcontratante` ASC),
  CONSTRAINT `fk_produto_contratante1`
    FOREIGN KEY (`contratante_idcontratante`)
    REFERENCES `mydb`.`contratante` (`idcontratante`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `mydb`.`LogProduto`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`LogProduto` (
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
    REFERENCES `mydb`.`produto` (`idproduto`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `mydb`.`produtoData`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`produtoData` (
  `idprodutoData` INT NOT NULL AUTO_INCREMENT,
  `dataInsercao` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `idproduto` VARCHAR(10) NOT NULL,
  PRIMARY KEY (`idprodutoData`),
  INDEX `fk_table1_produto1_idx` (`idproduto` ASC),
  CONSTRAINT `fk_table1_produto1`
    FOREIGN KEY (`idproduto`)
    REFERENCES `mydb`.`produto` (`idproduto`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`listaDesejos`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`listaDesejos` (
  `idlistaDesejos` INT NOT NULL AUTO_INCREMENT,
  `idprodutoData` INT NOT NULL,
  PRIMARY KEY (`idlistaDesejos`),
  INDEX `fk_listaDesejos_produtoData1_idx` (`idprodutoData` ASC),
  CONSTRAINT `fk_listaDesejos_produtoData1`
    FOREIGN KEY (`idprodutoData`)
    REFERENCES `mydb`.`produtoData` (`idprodutoData`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`cliente`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`cliente` (
  `idcliente` INT(11) NOT NULL AUTO_INCREMENT,
  `nome` VARCHAR(64) NULL DEFAULT NULL,
  `sobrenome` VARCHAR(64) NULL DEFAULT NULL,
  `sexo` ENUM('1', '2', '3') NULL DEFAULT NULL COMMENT '1 = Masculino\n2 = Feminino\n3 = Outro',
  `telefone` VARCHAR(11) NULL DEFAULT NULL,
  `email` VARCHAR(64) NOT NULL,
  `datanascimento` DATE NULL DEFAULT NULL,
  `senha` BLOB NOT NULL,
  `localizacao_idlocalizacao` INT(11) NULL DEFAULT NULL,
  `pontos` INT(11) NOT NULL DEFAULT '0' COMMENT 'Já é inicializado com 0',
  `listaDesejos_idlistaDesejos` INT NULL,
  PRIMARY KEY (`idcliente`, `email`),
  UNIQUE INDEX `email_UNIQUE` (`email` ASC),
  INDEX `fk_cliente_localizacao1_idx` (`localizacao_idlocalizacao` ASC),
  INDEX `fk_cliente_listaDesejos1_idx` (`listaDesejos_idlistaDesejos` ASC),
  CONSTRAINT `fk_cliente_localizacao1`
    FOREIGN KEY (`localizacao_idlocalizacao`)
    REFERENCES `mydb`.`localizacao` (`idlocalizacao`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_cliente_listaDesejos1`
    FOREIGN KEY (`listaDesejos_idlistaDesejos`)
    REFERENCES `mydb`.`listaDesejos` (`idlistaDesejos`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `mydb`.`feedBackCliente`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`feedBackCliente` (
  `idfeedBackCliente` INT(11) NOT NULL AUTO_INCREMENT,
  `titulo` VARCHAR(10) NOT NULL COMMENT 'Título do feedback concedido',
  `conteudo` VARCHAR(300) NOT NULL COMMENT 'Conteúdo do feedback informado pelo cliente',
  `emailCliente` VARCHAR(64) NOT NULL COMMENT 'Email do cliente que proveu o feedback',
  `idproduto` VARCHAR(10) NOT NULL COMMENT 'ID do produto relacionado',
  PRIMARY KEY (`idfeedBackCliente`),
  INDEX `fk_feedBackCliente_cliente1_idx` (`emailCliente` ASC),
  INDEX `fk_feedBackCliente_produto1_idx` (`idproduto` ASC),
  CONSTRAINT `fk_feedBackCliente_cliente1`
    FOREIGN KEY (`emailCliente`)
    REFERENCES `mydb`.`cliente` (`email`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_feedBackCliente_produto1`
    FOREIGN KEY (`idproduto`)
    REFERENCES `mydb`.`produto` (`idproduto`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `mydb`.`visualizacaoDeUsuario`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`visualizacaoDeUsuario` (
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
    REFERENCES `mydb`.`cliente` (`idcliente`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_visualizacaoDeUsuario_produto1`
    FOREIGN KEY (`produto_idproduto`)
    REFERENCES `mydb`.`produto` (`idproduto`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
