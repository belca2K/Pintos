# Introduzindo PintOS
<details>
<summary>Mudanças no ./src </summary>

- Para facilitar o export no src/utils depois de usar `make` usar `export PATH=$PATH:$(pwd)`, se nao quiser colocar no .bashrc/zshrc
Mas no src/threads(adicionado no src/threads/Makefile os comandos para executar o pintos mais facil. com GUI ou sem)
não precisa nem no `make check` do src/threads/build (se der algum erro oque foi modificado ta no src/tests/Make.tests:58);

- Para funcionar no Arch Linux modifiquei o src/Makefile.build:93 para ele reduzir o tamanho do loader.bin;

- Adicionado logica para ir executando os testes em especifico, no caso do threads, basicamente usa `make test TEST=<nome_do_test>`;

</details>

<details>
<summary>Recursos necessários </summary>


1. 
   <details>
   <summary>Make </summary>
   
   - O make serve para ajudar nossa vida. Não há uma obrigatoriedade de usá-lo, mas iremos;
   - Caso necessite instalá-lo, utilize o comando `sudo apt install make`;
   </details>

2.
   <details>
   <summary>GCC e GDB </summary>
   
   - Compilador e Depurador para C;
   - Caso necessite instalá-los, utilize os comandos `sudo apt install gcc` e `sudo apt install gdb`;
   </details>

3. 
   <details>
   <summary>Qemu </summary>
   
   - Recurso necessário para executar o sistema nos casos testes;
   - Caso necessite instalá-lo, utilize o comando `sudo apt install qemu-system-i386`;
   </details>

4. 
   <details>
   <summary>Boch </summary>
   
   - Alternativa mais rápida ao Qemu, entretanto não utilizaremos ele;
   - Se desejar saber como fazê-lo funcionar, acesse `https://web.stanford.edu/class/cs140/projects/pintos/pintos_12.html#SEC167`;
   </details>

</details>

<details>
<summary>WSL </summary>

- Caso opte por não fazer em sua máquina com Linux ou não possua permissão para baixar algum recurso no computador, siga o tutorial oficial oficial `https://learn.microsoft.com/pt-br/windows/wsl/install`;
</details>

<details>
<summary>Tutorial de comandos </summary>

- Uma vez que todos os recursos já estiverem instalados e sua branch desse repositório devidamente clonado, basta executar alguns `make`antes de começar a modificar o arquivo;
- Primeiramente, pelo terminal, vá até a pasta `src/utils` do repositório clonado e execute `make`;
- Em seguida, vá para a pasta `src/threads` e execute `make`;
- Então, na pasta `src/threads/build` você já pode executar `make check`, que executará, por padrão das modificações, o qemu de todos os testes;
- Caso deseje executar novamente, lembre-se de dar `make clean` antes de usar o proximo `make check`;
- Existe o comando `make check VERBOSE=1` fará com que tudo seja executado de maneira mais limpa em que cada teste aparecerá no terminal apenas durante sua execução;
- Você também pode executar o `make ngui TEST=nome_do_teste` na pasta `src/threads` após o make para executar apenas um teste específico;
- Caso você queira, pode ir na pasta `src/tests/threads` e modificar os arquivos da `tests.c` para que o `make check` execute apenas os casos que deseja;
- Todos os testes executados geram alguns arquivos relatando a saida e se passou ou não, que ficam na pasta `src/threads/build/tests`;
</details>

<details>
<summary>Observações </summary>

- Todas as operações descritas foram verificadas também no WSL;
- É possivel executar os comandos do Boch normalmente, porém se você não tiver ajustado ele, os resultados dos testes ficarão 27 de 27 falhas (testado no WSL);
- Há a opção de seguir o tutorial no arquivo `Exec_pintos.pdf` caso não queira fazer as modificações por si mesmo;
</details>

### Objetivos:
- [ ] Alarm Clock;
- [ ] Advanced Scheduler - Multi-Level Feedback Queue (mlfqs);
### Detalhamentos:

<details>
   <summary>Objetivos Principais</summary>
   
   Modificar o PintOS para que a lógica de sleep/wake funcione no alarme de forma devida e implementar devidamente a mlfqs. Nesse processo, os arquivos a serem modificados devem ser apenas os `src/device/timer.` e o `src/threads/thread.`;

</details>
<details>
   <summary>Alarm</summary>
   
   Reimplementar `timer_sleep()` no `device/time.c` que ta originalmente implementado com 'busy wait', que fica chamando `thread_yield()` enquanto o tempo não tiver passado;
   - Ideia:
   `Implementar um estado bloqueado para auxiliar e permitir corrigir o alarme.`
   - thread->wakeup_tick:
   `Variável que guarda o valor de tick que indica que a thread deve estar acordada (desbloqueada).`
   - thread_sleep():
   `Tiramos a espera ocupada e trocamos por um estado bloqueado de threads (sleep).`
   - wakeup_cmp():
   `Compara o valor entre o wakeup_tick das duas threads.`
   - thread_sleep():
   `Bloqueia as threads que não devem estar sendo executadas.`
   - thread_wakeup():
   `Desbloqueia as threads que devem estar sendo executadas.`


   

</details>
<details>
   <summary>Scheduler</summary>
   
   Na documentação oficial, ele sugere para dar opção de ter o mlfqs ou o por prioridade, ou seja, implementar ambos, porém nosso projeto não almeja a implementação do por prioridade; com o mlfqs as prioridades definidas pelas threads devem ser ignoradas e controladas pelo escalonador;
       
   [Fila esquema](https://www.google.com/url?sa=i&url=https://medium.com/@francescofranco_39234/multilevel-feedback-queue-3ae862436a95&psig=AOvVaw0uPvTNvKvDx0bKwYGvKyn_&ust=1718223750727000&source=images&cd=vfe&opi=89978449&ved=0CBIQjRxqFwoTCLD727Sw1IYDFQAAAAAdAAAAABAI)
   
   Segundo o apêndice que fala do scheduler devemos implementar o conceito de `avg_load`, `thread_nice` e o `cpu_recent_time`;
   
   O `avg_load` é a carga média do sistema levando em conta a quantidade de threads em ready_list, sem incluir thread ociosa:
   
   ```math
   avg = (\frac{59}{60}) * avg + (\frac{1}{60}) * (tamanho-da-ready-list)
   ```
   
   O `cpu_recent_time` é uma média móvel exponencial, específica de cada thread e que começa em 0, que serve como peso na hora de calcular a prioridade, que consiste em considerar uma função exponencial em que com o passar do temp os cpu-time antigos tenham pesos menores e os mais recentes os pesos maiores; todas as threads devem ter seu recent time recalculados 1 vez por segundo (timer_ticks() % TIMER_FREQ == 0) usando:
   
   ```math
   CpuTime = ( \frac{2 * avg}{2 * avg + 1} * CpuTime + nice) * 100
   ```
   
   O `nice` é específico de cada thread, há funções a se implementar e fazê-lo funcionar corretamente; ele deve estar entre -20 e 20 e vai servir para calcular a prioridade em que quanto mais positivo, menor a prioridade, que será calculada usando o `recent_time` (apenas se ele mudar) para alterar a thread de fila na mlfq, usando a fórmula:
   
   ```math
   p = floor(PriMax - (\frac{RecentCpuTime}{4}) - (nice * 2))
   ```
##### Pontos Flutuantes

   O kernel não suporta float nem double, então a documentação recomenda usar o formato de 17.14, 17 bits para a parte inteira e 14 para a fracionária; Para transformar reais nesses tipos basta multiplicar por 2^Q, onde Q é o numero de bits separado para a parte fracionária, e truncar para int, a documentação recomenda usar isso no recent cpu time e no avg, basicamente simulando operações em float usando inteiros (ver [aqui](https://www.scs.stanford.edu/23wi-cs212/pintos/pintos_7.html) como as operações podem ser feitas);
</details>

<details>
    <summary>Tests</summary>
   
   - Esses são todos os testes que serão executados quando usar o comando `make check` (caso não altere o `scr/tests/threads/tests.c`):
   - Obs: Pode passar `make check -j<numero de nproc>` para ele rodar os testes de forma paralela, para descobrir um valor bom de nproc é só rodar o comando `nproc`, que retorna o número de unidades de processamento disponíveis no sistema ou para o processo atual.
   
   | # | Teste | Implementada | Testada | Funcionando |
   |---|-----------|:-----------:|:-------:|:-----------:|
   | 1  | `alarm-single`|      OK     |    OK    |      OK      |
   | 2  | `alarm-multiple`|      OK     |    OK    |      OK      |
   | 3  | `alarm-simultaneous`|      OK     |    OK    |      OK      |
   | 4  | `alarm-priority`*|      ❌     |    ❌    |      ❌      |
   | 5  | `alarm-zero`|      OK     |    OK    |      OK      |
   | 6  | `alarm-negative`|      OK     |    OK    |      OK      |
   | 7  | `priority-change`*|      ❌     |    ❌    |      ❌      |
   | 8  | `priority-donate-one`*|      ❌     |    ❌    |      ❌      |
   | 9  | `priority-donate-multiple`*|      ❌     |    ❌    |      ❌      |
   | 10 | `priority-donate-multiple2`*|      ❌     |    ❌    |      ❌      |
   | 11 | `priority-donate-nest`*|      ❌     |    ❌    |      ❌      |
   | 12 | `priority-donate-sema`*|      ❌     |    ❌    |      ❌      |
   | 13 | `priority-donate-lower`*|      ❌     |    ❌    |      ❌      |
   | 14 | `priority-fifo`*|      ❌     |    ❌    |      ❌      |
   | 15 | `priority-preempt`*|      ❌     |    ❌    |      ❌      |
   | 16 | `priority-sema`*|      ❌     |    ❌    |      ❌      |
   | 17 | `priority-condvar`*|      ❌     |    ❌    |      ❌      |
   | 18 | `priority-donate-chain`*|      ❌     |    ❌    |      ❌      |
   | 19 | `mlfqs-load-1`|      ❌     |    ❌    |      ❌      |
   | 20 | `mlfqs-load-60`|      ❌     |    ❌    |      ❌      |
   | 21 | `mlfqs-load-avg`|      ❌     |    ❌    |      ❌      |
   | 22 | `mlfqs-recent-1`|      ❌     |    ❌    |      ❌      |
   | 23 | `mlfqs-fair-2`|      ❌     |    ❌    |      ❌      |
   | 24 | `mlfqs-fair-20`|      ❌     |    ❌    |      ❌      |
   | 25 | `mlfqs-nice-2`|      ❌     |    ❌    |      ❌      |
   | 26 | `mlfqs-nice-10`|      ❌     |    ❌    |      ❌      |
   | 27 | `mlfqs-block`|      ❌     |    ❌    |      ❌      |
   
   
</details>
<details>
   <summary>Detalhes</summary>
   
   - Para nossa aplicação do projeto de Infraestrutura de Software, nenhum dos testes de priority serão exigidos;
   - Os testes do alarm, quando executado, podem sinalizar que estão funcionando, porém, estão em espera ocupado, portanto não estão devidamente implementado;
   - Há esses vídeos de guia sobre o assunto, caso necessite de ajuda: `https://www.youtube.com/watch?v=myO2bs5LMak` e `https://www.youtube.com/watch?v=57r9OCN1EfA` (são aulas sobre a implementação do projeto completo do PintOS);
</details>


