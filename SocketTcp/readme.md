# Socket TCP 
  ### Como usar:
```sh
$ cd SocketTcp
$ python server_tcp.py
$ python client_tcp.py
```
- TIME
*Retorna a hora do sistema como uma String UTF no formato HH:MM:SS
- DATE
*Retorna a data do sistema como uma String UTF no formato DD/MM/AAAA
- FILES
*Retorna os arquivos da pasta definida por padrao (p. ex. /home/user/shared)
Formato:
*retorna um inteiro indicando o número de arquivos (int)* envia individualmente o nome de um arquivo como uma String UTF.
- DOWN nome-arquivo
*Faz o download do arquivo nome-arquivo
Formato:
    *retorna um inteiro 0 se nome não existe ou retorna o tamanho do arquivo (int) em bytes.
    *recebe byte a byte e grava em um diretório padrão
- EXIT
*Finaliza a conexão
