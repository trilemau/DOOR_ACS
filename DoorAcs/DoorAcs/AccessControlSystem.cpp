#include"AccessControlSystem.h"

AccessControlSystem::AccessControlSystem()
    : is_running_(false)
{
    is_running_ = true;
}

bool AccessControlSystem::IsRunning() const
{
    return is_running_;
}

void AccessControlSystem::MainLoop()
{
    while (is_running_)
    {
		try
		{
            const auto id = reader_.GetID();

            // Check if ID was found
            if (id.empty())
            {
                continue;
            }

            // Check if ID is valid
            if (IsIdValid(id))
            {
                std::cout << "Access granted!\n";
            }
            else
            {
                std::cout << "Access denied!\n";
            }
		}
		catch (const std::exception& e)
		{
            std::cout << "\n--- EXCEPTION!! ---\n";
            std::cout << "Sent data = " << reader_.GetDataString() << '\n';
            std::cout << "Number of bytes written = " << reader_.GetWrittenBytes() << '\n';
            std::cout << "Response data = " << reader_.GetResponseString() << '\n';
            std::cout << e.what() << '\n';
            reader_.FlushSerialPort();
		}

        std::cout << '\n';
    }
}

bool AccessControlSystem::IsIdValid(const string& id) const
{
    return access_granted_ids_.find(id) != access_granted_ids_.end();
}