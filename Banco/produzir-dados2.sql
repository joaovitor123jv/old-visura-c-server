INSERT INTO cliente(email, nome, sobrenome, localizacao_idlocalizacao, sexo, anonimo)
	SELECT 'hermes_o_deus@gmail.com', 'Hermes', 'Deus Grego', localizacao.idlocalizacao , '1', 0 from localizacao
    JOIN cidade on cidade.nome = 'Jataí'
    JOIN estado on estado.nome = 'GO'
    WHERE cidade.nome = 'Jataí' AND estado.nome = 'GO' AND localizacao.numero = 651;


#COMMENT 'EMPRESAS'

INSERT INTO contratante(nome, cnpj, plano, email, localizacao_idlocalizacao)
	SELECT 'Coca-Cola', 1524152142, 25, 'coca-cola@coca.com', localizacao.idlocalizacao from localizacao
    JOIN cidade ON cidade.idcidade = localizacao.cidade_idcidade
    JOIN estado ON estado.idestado = cidade.estado_idestado
    WHERE cidade.nome = 'Auriflama' AND estado.nome = 'SP' AND localizacao.numero = 157;
    
INSERT INTO contratante(nome, cnpj, plano, email, localizacao_idlocalizacao)
	SELECT 'Susheria', 645112010, 90, 'susheria@comida.com', localizacao.idlocalizacao from localizacao
    JOIN cidade ON cidade.idcidade = localizacao.cidade_idcidade
    JOIN estado ON estado.idestado = cidade.estado_idestado
    WHERE cidade.nome = 'Auriflama' AND estado.nome = 'SP' AND localizacao.numero = 65214;
    

#COMMENT 'PRODUTOS'

#INSERT INTO produto(contratante_idcontratante, duracao, idproduto)
#	SELECT contratante.idcontratante, 30, 'cocacolavc' FROM contratante
#	WHERE contratante.email = 'coca-cola@coca.com';

INSERT INTO produto(contratante_idcontratante, duracao, idproduto)
	SELECT contratante.idcontratante, 30, 'cocacomnos' FROM contratante
	WHERE contratante.email = 'coca-cola@coca.com';

INSERT INTO produto(contratante_idcontratante, duracao, idproduto)
	SELECT contratante.idcontratante, 40, 'coXaRamnRs' FROM contratante
	WHERE contratante.email = 'coca-cola@coca.com';
