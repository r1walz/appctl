#include <libapp/libapp.hh>

using namespace libapp;

err::obj
ctl::obj::register_data(app_db_t app_db, bool debug)
{
    auto data_dir = config.get("dir","data","/var/lib/app/index");
    auto app_data_dir = data_dir + "/" + app_db.name;
    auto app_data_file = app_data_dir + "/info";

    if (fs::is_exist(app_data_file))
        return err::obj(err::already_exist, app_data_file + " is already exist");

    if (!fs::is_exist(app_data_dir))
        fs::make_dir(app_data_dir);

    std::ofstream fptr(app_data_file);
    if (!fptr.good())
        return err::obj(0x123, "failed to open data file for write - " + app_data_file);

    std::string dpstr = "";
    for (auto a : app_db.depends)
        dpstr += " " + a;

    auto f_data = io::sprint(
        "name: ", app_db.name,"\n",
        "version: ", app_db.version,"\n",
        "release: ",app_db.release,"\n",
        "description: ",app_db.description,"\n",
        "depends: ", dpstr, "\n",
        "size: ", app_db.size,"\n",
        "installed: ", app_db.installed_time,"\n"
    );

    if (debug)
        io::info("datafile ", f_data);

    fptr.write(f_data.c_str(), f_data.size());

    fptr.close();

    return err::obj(0);
}