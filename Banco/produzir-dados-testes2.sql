-- INSERT INTO cliente(email, nome, sobrenome, localizacao_idlocalizacao, sexo, anonimo)
-- 	SELECT 'hermes_o_deus@gmail.com', 'Hermes', 'Deus Grego', localizacao.idlocalizacao , '1', 0 from localizacao
--     JOIN cidade on cidade.nome = 'Jataí'
--     JOIN estado on estado.nome = 'GO'
--     WHERE cidade.nome = 'Jataí' AND estado.nome = 'GO' AND localizacao.numero = 651;


#COMMENT 'EMPRESAS'

-- INSERT INTO contratante(nome, cnpj, plano, email, localizacao_idlocalizacao)
-- 	SELECT 'Coca-Cola', 1524152142, 25, 'coca-cola@coca.com', localizacao.idlocalizacao from localizacao
--     JOIN cidade ON cidade.idcidade = localizacao.cidade_idcidade
--     JOIN estado ON estado.idestado = cidade.estado_idestado
--     WHERE cidade.nome = 'Auriflama' AND estado.nome = 'SP' AND localizacao.numero = 157;
    
-- INSERT INTO contratante(nome, cnpj, plano, email, localizacao_idlocalizacao)
-- 	SELECT 'Susheria', 645112010, 90, 'susheria@comida.com', localizacao.idlocalizacao from localizacao
--     JOIN cidade ON cidade.idcidade = localizacao.cidade_idcidade
--     JOIN estado ON estado.idestado = cidade.estado_idestado
--     WHERE cidade.nome = 'Auriflama' AND estado.nome = 'SP' AND localizacao.numero = 65214;
    

#COMMENT 'PRODUTOS'

#INSERT INTO produto(contratante_idcontratante, duracao, idproduto)
#	SELECT contratante.idcontratante, 30, 'cocacolavc' FROM contratante
#	WHERE contratante.email = 'coca-cola@coca.com';

-- INSERT INTO produto(contratante_idcontratante, duracao, idproduto)
-- 	SELECT contratante.idcontratante, 30, 'cocacomnos' FROM contratante
-- 	WHERE contratante.email = 'coca-cola@coca.com';

-- INSERT INTO produto(contratante_idcontratante, duracao, idproduto)
-- 	SELECT contratante.idcontratante, 40, 'coXaRamnRs' FROM contratante
-- 	WHERE contratante.email = 'coca-cola@coca.com';



#COMMENT 'Cidades SP'
INSERT INTO cidade(nome, estado_idestado) SELECT 'Auriflama', estado.idestado from estado where estado.nome = 'SP';
INSERT INTO cidade(nome, estado_idestado) SELECT 'São_Paulo', estado.idestado from estado where estado.nome = 'SP';
INSERT INTO cidade(nome, estado_idestado) SELECT 'Botucatu', estado.idestado from estado where estado.nome = 'SP';
INSERT INTO cidade(nome, estado_idestado) SELECT 'Bauru', estado.idestado from estado where estado.nome = 'SP';
INSERT INTO cidade(nome, estado_idestado) SELECT 'Ibitinga', estado.idestado from estado where estado.nome = 'SP';
INSERT INTO cidade(nome, estado_idestado) SELECT 'São_José_do_Rio_Preto', estado.idestado from estado where estado.nome = 'SP';
INSERT INTO cidade(nome, estado_idestado) SELECT 'São_Luiz_do_Japiúba', estado.idestado from estado where estado.nome = 'SP';
INSERT INTO cidade(nome, estado_idestado) SELECT 'Guzolandia', estado.idestado from estado where estado.nome = 'SP';
INSERT INTO cidade(nome, estado_idestado) SELECT 'Campinas', estado.idestado from estado where estado.nome = 'SP';
INSERT INTO cidade(nome, estado_idestado) SELECT 'Rio_Branco', estado.idestado from estado where estado.nome = 'SP';
INSERT INTO cidade(nome, estado_idestado) SELECT 'São_Carlos', estado.idestado from estado where estado.nome = 'SP';
INSERT INTO cidade(nome, estado_idestado) SELECT 'Araçatuba', estado.idestado from estado where estado.nome = 'SP';
INSERT INTO cidade(nome, estado_idestado) SELECT 'Birigui', estado.idestado from estado where estado.nome = 'SP';
INSERT INTO cidade(nome, estado_idestado) SELECT 'Fernandópolis', estado.idestado from estado where estado.nome = 'SP';
INSERT INTO cidade(nome, estado_idestado) SELECT 'Pedranópolis', estado.idestado from estado where estado.nome = 'SP';


#COMMENT 'Cidades GO'

INSERT INTO cidade(nome, estado_idestado) SELECT 'Jataí', estado.idestado from estado where estado.nome = 'GO';
INSERT INTO cidade(nome, estado_idestado) SELECT 'Rio_Verde', estado.idestado from estado where estado.nome = 'GO';
INSERT INTO cidade(nome, estado_idestado) SELECT 'Quirinópolis', estado.idestado from estado where estado.nome = 'GO';
INSERT INTO cidade(nome, estado_idestado) SELECT 'Mineiros', estado.idestado from estado where estado.nome = 'GO';


#COMMENT 'Cidades MG'

INSERT INTO cidade(nome, estado_idestado) SELECT 'Belo_Horizonte', estado.idestado from estado where estado.nome = 'MG';
INSERT INTO cidade(nome, estado_idestado) SELECT 'Uberlândia', estado.idestado from estado where estado.nome = 'MG';
INSERT INTO cidade(nome, estado_idestado) SELECT 'Uberaba', estado.idestado from estado where estado.nome = 'MG';
INSERT INTO cidade(nome, estado_idestado) SELECT 'Frutal', estado.idestado from estado where estado.nome = 'MG';


#COMMENT 'Localizacoes'

INSERT INTO localizacao(cep, cidade_idcidade, numero)
 SELECT '15350000', cidade.idcidade, 15 from cidade
    JOIN estado on estado.idestado = cidade.estado_idestado
    WHERE cidade.nome = 'Auriflama' AND estado.nome = 'SP';

INSERT INTO localizacao(cep, cidade_idcidade, numero)
 SELECT '15350000', cidade.idcidade, 412 from cidade
    JOIN estado on estado.idestado = cidade.estado_idestado
    WHERE cidade.nome = 'Auriflama' AND estado.nome = 'SP';

INSERT INTO localizacao(cep, cidade_idcidade, numero)
 SELECT '15350000', cidade.idcidade, 65214 from cidade
    JOIN estado on estado.idestado = cidade.estado_idestado
    WHERE cidade.nome = 'Auriflama' AND estado.nome = 'SP';

INSERT INTO localizacao(cep, cidade_idcidade, numero)
 SELECT '15350000', cidade.idcidade, 157 from cidade
    JOIN estado on estado.idestado = cidade.estado_idestado
    WHERE cidade.nome = 'Auriflama' AND estado.nome = 'SP';
    
INSERT INTO localizacao(cep, cidade_idcidade, numero)
 SELECT '15350000', cidade.idcidade, 157 from cidade
    JOIN estado on estado.idestado = cidade.estado_idestado
    WHERE cidade.nome = 'Ibitinga' AND estado.nome = 'SP';

INSERT INTO localizacao(cep, cidade_idcidade, numero)
 SELECT '65210000', cidade.idcidade, 1546 from cidade
    JOIN estado on estado.idestado = cidade.estado_idestado
    WHERE cidade.nome = 'Jataí' AND estado.nome = 'GO';

INSERT INTO localizacao(cep, cidade_idcidade, numero)
 SELECT '65210001', cidade.idcidade, 651 from cidade
    JOIN estado on estado.idestado = cidade.estado_idestado
    WHERE cidade.nome = 'Jataí' AND estado.nome = 'GO';
    
    
#COMMENT 'CLIENTE'

-- INSERT INTO cliente(email, nome, sobrenome, sexo, senha)
--     VALUES('interface@servidor', 'Interface', 'Senatauri Enterprise', '3', 'senatauri');
    -- SELECT 'interface@servidor', 'Interface', 'Senatauri Enterprise', '3', 'senatauri' 
    -- from localizacao
    -- JOIN cidade on cidade.nome = 'Auriflama'
    -- JOIN estado on estado.nome = 'SP'
    -- WHERE cidade.nome = 'Auriflama' AND estado.nome = 'SP' AND localizacao.numero = 65214;
    
INSERT INTO cliente(email, nome, sobrenome, localizacao_idlocalizacao, sexo, senha)
 SELECT 'jv.joaovitoras@gmail.com', 'João Vitor', 'Antoniassi Segantin', localizacao.idlocalizacao , '1', 'nulo' from localizacao
    JOIN cidade on cidade.nome = 'Auriflama'
    JOIN estado on estado.nome = 'SP'
    WHERE cidade.nome = 'Auriflama' AND estado.nome = 'SP' AND localizacao.numero = 65214;
    
INSERT INTO cliente(email, nome, sobrenome, localizacao_idlocalizacao, sexo, senha)
 SELECT 'carlos@gmail.com', 'Carlos Gabriel', 'Sei lá o que Stédile', localizacao.idlocalizacao , '1', 'nulo' from localizacao
    JOIN cidade on cidade.nome = 'Jataí'
    JOIN estado on estado.nome = 'GO'
    WHERE cidade.nome = 'Jataí' AND estado.nome = 'GO' AND localizacao.numero = 1546;

INSERT INTO cliente(email, nome, sobrenome, localizacao_idlocalizacao, sexo, senha)
 SELECT 'wemerson@gmail.com', 'Wemerson', 'Coroa Ingles', localizacao.idlocalizacao , '1', 'nulo' from localizacao
    JOIN cidade on cidade.nome = 'Jataí'
    JOIN estado on estado.nome = 'GO'
    WHERE cidade.nome = 'Jataí' AND estado.nome = 'GO' AND localizacao.numero = 1546;



#COMMENT 'EMPRESAS'

INSERT INTO contratante(nome, cnpj, plano, email, localizacao_idlocalizacao, senha)
 SELECT 'Coca-Cola', 1524152142, 25, 'coca-cola@coca.com', localizacao.idlocalizacao, 'coca' from localizacao
    JOIN cidade ON cidade.idcidade = localizacao.cidade_idcidade
    JOIN estado ON estado.idestado = cidade.estado_idestado
    WHERE cidade.nome = 'Auriflama' AND estado.nome = 'SP' AND localizacao.numero = 157;
    
INSERT INTO contratante(nome, cnpj, plano, email, localizacao_idlocalizacao, senha)
 SELECT 'Susheria', 645112010, 90, 'susheria@comida.com', localizacao.idlocalizacao, 'sushi' from localizacao
    JOIN cidade ON cidade.idcidade = localizacao.cidade_idcidade
    JOIN estado ON estado.idestado = cidade.estado_idestado
    WHERE cidade.nome = 'Auriflama' AND estado.nome = 'SP' AND localizacao.numero = 65214;
    

#COMMENT 'PRODUTOS'

INSERT INTO produto(contratante_idcontratante, duracao, idproduto)
 SELECT contratante.idcontratante, 30, 'cocacolavc' FROM contratante
 WHERE contratante.email = 'coca-cola@coca.com';

INSERT INTO produto(contratante_idcontratante, duracao, idproduto)
 SELECT contratante.idcontratante, 30, 'cocacomnos' FROM contratante
 WHERE contratante.email = 'coca-cola@coca.com';
    
INSERT INTO produto(contratante_idcontratante, duracao, idproduto)
 SELECT contratante.idcontratante, 30, 'sushilokoN' FROM contratante
 WHERE contratante.email = 'susheria@comida.com';    

INSERT INTO produto(contratante_idcontratante, duracao, idproduto)
 SELECT contratante.idcontratante, 40, 'coXaRamnRs' FROM contratante
 WHERE contratante.email = 'coca-cola@coca.com';
    
INSERT INTO produto(contratante_idcontratante, duracao, idproduto)
 SELECT contratante.idcontratante, 30, 'maisumcoca' FROM contratante
 WHERE contratante.email = 'coca-cola@coca.com';

INSERT INTO produto(contratante_idcontratante, duracao, idproduto)
 SELECT contratante.idcontratante, 30, 'sushilokoM' FROM contratante
 WHERE contratante.email = 'susheria@comida.com';


INSERT INTO produto(contratante_idcontratante, duracao, idproduto, descricao, tipo, nomeProduto)
 SELECT contratante.idcontratante, 10, 'teste11111', 'Pacote_de_teste_1', '1', 'Teste_1' FROM contratante
 WHERE contratante.email = 'susheria@comida.com';

INSERT INTO produto(contratante_idcontratante, duracao, idproduto, descricao, tipo, nomeProduto)
 SELECT contratante.idcontratante, 30, 'teste22222', 'Pacote_de_teste_2', '1', 'Teste_2' FROM contratante
 WHERE contratante.email = 'susheria@comida.com';





