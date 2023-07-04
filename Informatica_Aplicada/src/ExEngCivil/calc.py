class CalculadoraOrcamento:
    def _init_(self):
        self.valor_materiais = 0.0
        self.valor_mao_de_obra = 0.0
        self.valor_equipamentos = 0.0
        self.valor_despesas_administrativas = 0.0
        self.valor_canteiro_de_obras = 0.0

    def custo_materiais(self):
        self.valor_materiais = float(input("Digite o valor dos materiais: "))

    def custo_mao_de_obra(self):
        self.valor_mao_de_obra = float(input("Digite o valor da mão de obra: "))

    def custo_equipamentos(self):
        self.valor_equipamentos = float(input("Digite o valor dos equipamentos: "))

    def custo_despesas_administrativas(self):
        self.valor_despesas_administrativas = float(input("Digite o valor das despesas administrativas: "))

    def custo_canteiro_de_obras(self):
        self.valor_canteiro_de_obras = float(input("Digite o valor do canteiro de obras: "))

    def calcula_custo_total(self, desconto=0):
        total = (
            self.valor_materiais
            + self.valor_mao_de_obra
            + self.valor_equipamentos
            + self.valor_despesas_administrativas
            + self.valor_canteiro_de_obras
        )
        desconto_total = total * (desconto / 100)
        custo_total = total - desconto_total
        return custo_total

    def calcula_custo_medio_por_metro_quadrado(self, desconto=0, area=0):
        custo_total = self.calcula_custo_total(desconto)
        custo_medio_por_metro_quadrado = custo_total / area
        return custo_medio_por_metro_quadrado

    def apresenta_resultados(self, desconto=0, area=0):
        custo_total = self.calcula_custo_total(desconto)
        print("O custo total da obra é R${:.2f}".format(custo_total))
        custo_medio_por_metro_quadrado = self.calcula_custo_medio_por_metro_quadrado(desconto, area)
        print("O custo médio por metro quadrado é R${:.2f}".format(custo_medio_por_metro_quadrado))


calcu = CalculadoraOrcamento()
continuar = True
while continuar:
    desconto = float(input("Digite o valor do desconto (0 para nenhum desconto): "))
    calcu.custo_materiais()
    calcu.custo_mao_de_obra()
    calcu.custo_equipamentos()
    calcu.custo_despesas_administrativas()
    calcu.custo_canteiro_de_obras()
    area = float(input("Digite a área da obra em metros quadrados: "))
    calcu.apresenta_resultados(desconto, area)

    resposta = input("Deseja calcular outro orçamento? (s/n): ")
    if resposta.lower() != "s":
        continuar = False