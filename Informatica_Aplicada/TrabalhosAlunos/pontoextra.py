import tkinter as tk
import random

def trocar_nao():
    # Trocar a posição da resposta "Não"
    nova_linha = random.randint(1, 2)
    nova_coluna = random.randint(0, 2)
    nao_button.grid(row=nova_linha, column=nova_coluna)
    root.after(500, trocar_nao)  # Chama a função novamente após 500ms

def aceitar_pedido():
    nao_button.grid_remove()  # Esconder o botão "Não"
    resposta_label.config(text="Muito obrigado, professor!", fg="green", font=("Helvetica", 16, "bold"))
    resposta_label.grid(row=2, column=1)

# Configuração da janela
root = tk.Tk()
root.title("Pedido de Ponto Extra")
root.configure(bg="#E0F7FA")  # Cor de fundo suave

# Criar labels e botões com estilo
pergunta_label = tk.Label(root, text="Professor, você pode nos dar 1 ponto extra?", bg="#E0F7FA", font=("Helvetica", 16, "bold"))
pergunta_label.grid(row=0, column=0, columnspan=3, pady=20)

nao_button = tk.Button(root, text="Não", bg="#FF6B6B", fg="white", font=("Helvetica", 14), relief="raised")
nao_button.grid(row=1, column=0, padx=10, pady=10)

sim_button = tk.Button(root, text="Sim", command=aceitar_pedido, bg="#4CAF50", fg="white", font=("Helvetica", 14), relief="raised")
sim_button.grid(row=1, column=2, padx=10, pady=10)

resposta_label = tk.Label(root, text="", fg="red", font=("Helvetica", 14))
resposta_label.grid(row=2, column=1)

# Iniciar o movimento do botão "Não"
trocar_nao()

# Iniciar a interface
root.mainloop()
