-- CreateTable
CREATE TABLE `leitura` (
    `id_leitura` INTEGER NOT NULL AUTO_INCREMENT,
    `id_sensor` INTEGER NOT NULL,
    `timestamp` VARCHAR(200) NOT NULL,
    `dados` VARCHAR(100) NOT NULL,

    INDEX `id_sensor`(`id_sensor`),
    PRIMARY KEY (`id_leitura`)
) DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

-- CreateTable
CREATE TABLE `sensor` (
    `id_sensor` INTEGER NOT NULL AUTO_INCREMENT,
    `id_SS` INTEGER NOT NULL,
    `nome_sensor` VARCHAR(100) NOT NULL,
    `estado` VARCHAR(100) NOT NULL,

    INDEX `id_SS`(`id_SS`),
    PRIMARY KEY (`id_sensor`)
) DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

-- CreateTable
CREATE TABLE `sistema_sensor` (
    `id_SS` INTEGER NOT NULL,
    `nome_sistema_sensor` VARCHAR(100) NOT NULL,
    `tipo` VARCHAR(100) NOT NULL,
    `estado` VARCHAR(100) NOT NULL,

    PRIMARY KEY (`id_SS`)
) DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

-- AddForeignKey
ALTER TABLE `leitura` ADD CONSTRAINT `leitura_ibfk_1` FOREIGN KEY (`id_sensor`) REFERENCES `sensor`(`id_sensor`) ON DELETE NO ACTION ON UPDATE NO ACTION;

-- AddForeignKey
ALTER TABLE `sensor` ADD CONSTRAINT `sensor_ibfk_1` FOREIGN KEY (`id_SS`) REFERENCES `sistema_sensor`(`id_SS`) ON DELETE NO ACTION ON UPDATE NO ACTION;
