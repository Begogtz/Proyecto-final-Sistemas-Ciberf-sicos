# Control de versiones

## Regla básica

Cada cambio funcional debe quedar asociado a un commit que describa qué se agregó, corrigió o validó. No se debe crear historial ficticio para trabajo anterior; el control de versiones inicia con el estado real que se incorpora al repositorio.

## Convención recomendada

- `feat:` nueva funcionalidad.
- `fix:` corrección de software.
- `test:` prueba o validación.
- `docs:` documentación.
- `refactor:` reorganización sin cambiar funcionalidad.
- `chore:` mantenimiento del proyecto.

## Commits sugeridos para incorporar este estado

Al subir por primera vez este proyecto, se puede usar un único commit base:

```text
feat: add S32K344 dual RoboClaw motor-control firmware
```

Después, los siguientes cambios deben registrarse cuando realmente ocurran. Por ejemplo:

```text
test: validate four-motor RoboClaw bench test
feat: add Mecanum wheel mapping
feat: add NUC to S32K344 command interface
fix: handle RoboClaw communication timeout safely
docs: add AMR electrical and software test evidence
```

## Evidencia

Cuando un commit corresponda a una prueba física, el registro de calidad debe incluir el identificador del commit o el nombre del archivo de evidencia para relacionar software y resultado experimental.
