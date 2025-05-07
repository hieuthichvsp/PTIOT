# 1 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\KT\\sketch.ino"
user{
    id int pk,
    name varchar(50),
    password varchar(50),
    role_id int,
    status_id int,
    gmail varchar(50),
    phone varchar(15),
    label varchar(50),
} status{
    id int pk,
    name varchar(50),
} role{
    id int pk,
    name varchar(50),
} capture_img{
    id int pk,
    user_id int,
    img_path varchar(255),
    created_at datetime,
    result varchar(50),
    confidence_core float,
    device_id int,
} device
{
    id int pk,
        name varchar(50),
        status_id int,
        created_at datetime,
}
