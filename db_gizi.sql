create table users (
	id int auto_increment primary key,
	username varchar(100) not null,
	password varchar(100) not null,
	role varchar (10) not null
);

create table makanan (
	id_makanan int auto_increment primary key,
	nama_makanan varchar(100) not null,
	kategori varchar(100) not null,
	kalori float not null,
	protein float not null,
	karbohidrat float not null,
	lemak float not NULL
);

crate table log(
	id_log int auto_increment primary key,
	id_user int not null,
	nama_kegiatan varchar(100) not null,
	tanggal datetime not null,
	role varchar(10) not null,
	foreign key (id_user) references users(id),
	foreign key (nama_kegiatan) references makanan(nama_makanan),
	foreign key (role) references users(role)
);

insert into users values (1, 'fina', '016', 'admin'), (2, 'komang', '003', 'admin'), (3, 'zaki', '020', 'admin'), (4, 'senku', '1234', 'user');
insert into makanan values (1, 'Nasi putih', 'Makanan utama', 130, 2.7, 28.2, 0.3), (2, 'Ayam goreng', 'Lauk pauk', 260, 27.3, 1.8, 16.8), (3, 'Sate ayam', 'Lauk pauk', 200, 20, 15, 10), (4, 'Rendang daging', 'Lauk pauk', 468, 47, 10.5, 30), (5, 'Pisang rebus', 'Cemilan', 110, 1.2, 28, 0.3); 
insert into log (id_user, nama_kegiatan, tanggal, role) values (1, 'login', NOW(), 'admin'), (1, 'Request makanan', NOW(), 'user'), (2, 'Register', NOW(), 'user'), (2, 'Request makanan', NOW(), 'user'), (3, 'Delete data', NOW(), 'admin');

select * from users;
select * from makanan;
<<<<<<< HEAD
select * from log;
=======

CREATE TABLE request_user (
    id_request INT AUTO_INCREMENT PRIMARY KEY,
    id_user INT,
    nama_makanan_req VARCHAR(100),
    status_request VARCHAR(20) DEFAULT 'Pending',
    FOREIGN KEY (id_user) REFERENCES users(id)
);

CREATE TABLE log_user (
    id_log INT AUTO_INCREMENT PRIMARY KEY,
    id_user INT,
    aktivitas VARCHAR(255),
    waktu TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (id_user) REFERENCES users(id)
);

INSERT INTO log_user (id_user, aktivitas) VALUES (4, 'Melakukan request makanan: Gulai Kambing');
INSERT INTO log_user (id_user, aktivitas) VALUES (4, 'Login ke sistem aplikasi gizi');
INSERT INTO log_user (id_user, aktivitas) VALUES (2, 'Admin Komang memperbarui data makanan');

INSERT INTO makanan (id_makanan, nama_makanan, kategori, kalori, protein, karbohidrat, lemak) VALUES 
(8, 'Tempe Goreng', 'Lauk pauk', 120, 10, 8, 7),
(9, 'Soto Ayam', 'Makanan utama', 312, 20, 30, 12),
(11, 'Apel', 'Buah', 52, 0.3, 14, 0.2),
(12, 'Telur Rebus', 'Lauk pauk', 77, 6, 1.1, 5);

CREATE TABLE IF NOT EXISTS manajemen_rekomendasi (
    id_rekomendasi INT AUTO_INCREMENT PRIMARY KEY,
    kategori_bmi VARCHAR(50),
    saran_diet VARCHAR(255)
);

INSERT INTO manajemen_rekomendasi (kategori_bmi, saran_diet) VALUES
('Underweight', 'Tingkatkan asupan kalori, konsumsi protein tinggi seperti telur dan ayam'),
('Normal', 'Pertahankan pola makan seimbang dengan gizi lengkap'),
('Overweight', 'Kurangi karbohidrat dan lemak, perbanyak sayur dan buah'),
('Obesitas', 'Diet ketat rendah kalori, konsultasi dengan ahli gizi');

INSERT INTO request_user (id_user, nama_makanan_req, status_request) VALUES
(4, 'Gulai Kambing', 'Pending'),
(4, 'Bakso Sapi', 'Pending');
>>>>>>> a0d542daca9ed3bb2269ddb2ac4fda5cbe81d587
