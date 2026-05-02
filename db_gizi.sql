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

insert into users values (1, 'fina', '016', 'admin'), (2, 'komang', '003', 'admin'), (3, 'zaki', '020', 'admin'), (4, 'senku', '1234', 'user');
insert into makanan values (1, 'Nasi putih', 'Makanan utama', 130, 2.7, 28.2, 0.3), (2, 'Ayam goreng', 'Lauk pauk', 260, 27.3, 1.8, 16.8), (3, 'Sate ayam', 'Lauk pauk', 200, 20, 15, 10), (4, 'Rendang daging', 'Lauk pauk', 468, 47, 10.5, 30), (5, 'Pisang rebus', 'Cemilan', 110, 1.2, 28, 0.3); 

select * from users;
select * from makanan;