n = int("123334249275464637138917520847189683399")
coefficients = [199, 172, 201, 226, 65, 95, 214, 43, 243, 165, 126, 115, 248, 79, 201, 92]

reconstructed_n = 0
for i, coeff in enumerate(coefficients):
    step_value = coeff * (256 ** i)
    print(f"Étape {i + 1} : {coeff} * 256^{i} = {step_value}")
    reconstructed_n += step_value

print(f"Nombre reconstruit : {reconstructed_n}")
